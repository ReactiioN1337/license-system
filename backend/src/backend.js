/**
 * @author    ReactiioN
 * @copyright 2019, reactiion.net
 * @license   MIT
 */
'use strict'

module.exports = (app, config, db) => {
  const cryptlib                 = require('./cryptlib'),
        enums                    = require('./enum'),
        Promise                  = require('bluebird'),
        moment                   = require('moment'),
        fs                       = require('./filesystem'),
        {APICode, ClientRequest} = enums,
        {KeyPair}                = cryptlib

  this._keyPair = new KeyPair(config.socket.publicKey, config.socket.privateKey)

  this._encrypt = (plain, publicKey = undefined) => {
    let kp = null
    if (publicKey !== undefined) {
      kp = new KeyPair(publicKey)
    } else {
      kp = this._keyPair
    }
    return cryptlib.sealedboxEncrypt(plain, kp)
  }

  this._decrypt = (cipher) => {
    return cryptlib.sealedboxDecrypt(cipher, this._keyPair)
  }

  this._handleUserLogin = (client) => {
    return db.findUser(client.data.username)
    .then((user) => {
      // permanent banned
      if (user.ban_date === -1) {
        return Promise.reject(APICode.reject(APICode.UserIsBanned))
      }

      const promises = []
      if (user.ban_date > 0) {
        const endDate = moment.unix(user.ban_date),
              now     = moment()

        if (now.isSameOrBefore(endDate)) {
          return Promise.reject(APICode.reject(APICode.UserIsBanned))
        }
        promises.push(db.setUserBan(user.username, 0))
      }
      if (user.hwid_hash.length === 0) {
        promises.push(db.setUserHwidHash(user.username, client.data.hwid).then(() => user.hwid_hash = client.data.hwid))
      }
      if (promises.length === 0) {
        promises.push(Promise.resolve())
      }
      return Promise.all(promises)
      .then(() => {
        if (user.hwid_hash !== client.data.hwid) {
          return Promise.reject(APICode.reject(APICode.UserHwidMismatch))
        }
      })
      .then(() => {return {name: user.username, id: user.id}})
    })
    .catch((err) => {
      if (err.code === APICode.SQL_NullRows) {
        return Promise.reject(APICode.reject(APICode.UserNotFound))
      }
      return Promise.reject(err)
    })
  }

  this._handleGetSubs = (client) => {
    return db.findSubscriptions(client.data.user_id)
    .then((subs) => {
      const promises = [],
            response = []
      subs.forEach((sub) => {
        let valid = true
        if (sub.date_begin !== 0 && sub.date_end !== 0) {
          if (sub.date_begin !== -1 && sub.date_end !== -1) {
            const dateBegin = moment.unix(sub.date_begin),
                  dateEnd   = moment.unix(sub.date_end),
                  now       = moment()

            if (now.isSameOrAfter(dateBegin) && now.isSameOrBefore(dateEnd)) {
              valid = true
            }
          }
          if (valid) {
            promises.push(db.findProductById(sub.product_id).then((product) => {
              response.push({name: product.name, id: product.id, until: sub.date_end})
            }))
          }
        }
      })

      return Promise.all(promises)
      .then(() => {return response})
    })
    .catch((err) => {
      if (err.code === APICode.SQL_NullRows) {
        return Promise.reject(APICode.reject(APICode.UserHasNoSubscriptions))
      }
      return Promise.reject(err)
    })
  }

  this._handleGetProduct = (client) => {
    return db.findProductById(client.data.product_id)
    .then((product) => {
      if (product.active !== 1) {
        return Promise.reject(APICode.reject(APICode.ProductNotActive))
      }
      if (product.file_name.length === 0) {
        return Promise.reject(APICode.reject(APICode.ProductHasNoFileName))
      }
      return fs.readFile(`data/bin/${product.file_name}`)
    })
    .then((fileContents) => {return fileContents})
    .catch((err) => {
      if (err.code === APICode.SQL_NullRows) {
        return Promise.reject(APICode.reject(APICode.ProductNotFound))
      }
      if (err.code === APICode.FailedToReadFile) {
        return Promise.reject(APICode.reject(APICode.ProductHasNoFile))
      }
      return Promise.reject(err)
    })
  }

  this._handleClientPostRequest = (client) => {
    switch (client.request) {
      case ClientRequest.Login:      return this._handleUserLogin(client)
      case ClientRequest.GetSubs:    return this._handleGetSubs(client)
      case ClientRequest.GetProduct: return this._handleGetProduct(client)
    }
  }

  this.handleClientPostRequest = (req, res) => {
    if (!req.body || !req.body.data || !cryptlib.isBase64(req.body.data)) {
      return res.sendStatus(400) // Bad Request
    }

    try {
      this._decrypt(req.body.data)
      .then((decryptedClientData) => JSON.parse(decryptedClientData))
      .then((client) => {
        this._handleClientPostRequest(client)
        .then((responseData) => this.clientAnswer(res, client, APICode.Success, responseData))
        .catch((err) => this.clientAnswer(res, client, err.code))
      })
      .catch(() => res.sendStatus(400))
    } catch (err) {
      res.sendStatus(400)
    }
  }

  this.clientAnswer = (res, client, code, data = undefined) => {
    const obj = {code}
    if (data !== undefined) {
      obj['response'] = data
    }
    this._encrypt(JSON.stringify(obj), client.public_key)
    .then((encryptedData) => this.writeResponse(res, encryptedData))
    .catch(() => res.sendStatus(409)) // Conflict
  }

  this.writeResponse = (res, data) => {
    res.set('Content-Type', 'text-plain')
    res.write(data)
    res.end()
    return this
  }

  require('./rest')(app, config, db)

  return this
}
