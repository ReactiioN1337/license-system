/**
 * @author    ReactiioN
 * @copyright 2019, reactiion.net
 * @license   MIT
 */
'use strict'

const fs                  = require('./filesystem'),
      Promise             = require('bluebird'),
      {APICode, APILevel} = require('./enum')

const saveFileContents = (fileObj) => {
  const binPath = 'data/bin'
  return fs.pathExists(binPath)
  .then((exists) => {
    if (exists !== true) {
      return fs.mkdirp(binPath)
    }
  }).then(() => fs.writeFile(`${binPath}/${fileObj.name}`, fileObj.contents))
}

module.exports = (app, config, db) => {
  const webCfg        = config.web,
        requests      = webCfg.requests,
        makePath      = (name) => {return `/${config.web.requestPath}/${name}`}
  const makeAnswer    = (res, data) => {
    res.set('Content-Type', 'text-plain')
    res.write(typeof(data) === 'object' ? JSON.stringify(data) : data)
    res.end()
  }
  const makeAnswerRaw = (res, id, data = undefined, message = undefined) => {
    const obj = {code: id, msg: message || APICode.toString(id)}
    if (data !== undefined) {
      obj.response = data
    }
    makeAnswer(res, obj)
  }
  const randomString = () => {return Math.random().toString(36).slice(-8) + Math.random().toString(36).slice(-8)}

  app.post(makePath(requests.addUser), (req, res) => {
    if (!req.body) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }

    const {name, token} = req.body
    if (name === undefined || token === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (name.length === 0 || token.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    return db.getUserlistCount()
    .then((numUsers) => {
      return db.findUser(name)
      .then(() => makeAnswerRaw(res, APICode.UserAlreadyExists))
      .catch((err) => {
        if (err.code !== APICode.SQL_NullRows) {
          return makeAnswer(res, err)
        }
        return db.addUser(name, numUsers === 0 ? APILevel.Admin : APILevel.Disabled, randomString(), numUsers === 0 ? 0 : -1)
        .then(() => makeAnswerRaw(res, APICode.Success))
      })
    })
    .catch((err) => makeAnswer(res, err))
  })

  app.post(makePath(requests.delUser), (req, res) => {
    if (!req.body) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }

    const {name, token} = req.body
    if (name === undefined || token === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (name.length === 0 || token.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    return db.removeUser(name)
    .then(() => makeAnswerRaw(res, APICode.Success))
    .catch((err) => {
      if (err.code === APICode.SQL_NullRows) {
        return makeAnswerRaw(res, APICode.UserNotFound)
      }
      return makeAnswer(res, err)
    })
  })

  app.post(makePath(requests.banUser), (req, res) => {
    if (!req.body) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }

    const {name, token, date} = req.body
    if (name === undefined || token === undefined || date === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (name.length === 0 || token.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    return db.setUserBan(name, date)
    .then(() => makeAnswerRaw(res, APICode.Success))
    .catch((err) => makeAnswer(res, err))
  })

  app.post(makePath(requests.resetHwid), (req, res) => {
    if (!req.body) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }

    const {name, token} = req.body
    if (name === undefined || token === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (name.length === 0 || token.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    return db.setUserHwidHash(name, '')
    .then(() => makeAnswerRaw(res, APICode.Success))
    .catch((err) => makeAnswer(res, err))
  })

  app.post(makePath(requests.setApiAccess), (req, res) => {
    if (!req.body) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }

    const {name, token, level} = req.body
    if (name === undefined || token === undefined || level === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (name.length === 0 || token.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    return db.setUserApiAccess(name, level)
    .then(() => makeAnswerRaw(res, APICode.Success))
    .catch((err) => makeAnswer(res, err))
  })

  app.post(makePath(requests.addProduct), (req, res) => {
    if (!req.body) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }

    const {name, token, active, file_name} = req.body
    if (name === undefined || token === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (name.length === 0 || token.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    return db.addProduct(name, active || false, file_name || '')
    .then(() => makeAnswerRaw(res, APICode.Success))
    .catch((err) => makeAnswer(res, err))
  })

  app.post(makePath(requests.delProduct), (req, res) => {
    const {name, token} = req.body
    if (name === undefined || token === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (name.length === 0 || token.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    return db.removeProduct(name)
    .then(() => makeAnswerRaw(res, APICode.Success))
    .catch((err) => {
      if (err.code === APICode.SQL_NullRows) {
        return makeAnswerRaw(res, APICode.ProductNotFound)
      }
      return makeAnswer(res, err)
    })
  })

  app.post(makePath(requests.modifyProduct), (req, res) => {
    const {name, token, active, file} = req.body
    if (name === undefined || token === undefined || (active === undefined && file === undefined)) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (name.length === 0 || token.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }

    const promises = []
    if (active !== undefined) {
      promises.push(db.setProductActive(name, active))
    }
    if (file !== undefined) {
      if (file.name === undefined) {
        return makeAnswerRaw(res, APICode.CorruptedRequest)
      }
      promises.push(db.setProductFileName(name, file.name))

      if (file.contents !== undefined && typeof(file.contents) === 'string') {
        promises.push(saveFileContents(file))
      }
    }
    return Promise.all(promises)
    .then(() => makeAnswerRaw(res, APICode.Success))
    .catch((err) => makeAnswer(res, err))
  })

  app.post(makePath(requests.addSub), (req, res) => {
    const {token, username, product, date} = req.body
    if (token === undefined || username === undefined || product === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token.length === 0 || username.length === 0 || product.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }

    let dateBegin = 0, dateEnd = 0
    if (date !== undefined) {
      dateBegin = date.begin || 0
      dateEnd   = date.end   || 0
    }
    return db.findUser(username)
    .then((user) => {
      return db.findProduct(product)
      .then((productData) => db.addSubscription(user.id, productData.id, dateBegin, dateEnd))
    })
    .then(() => makeAnswerRaw(res, APICode.Success))
    .catch((err) => makeAnswer(res, err))
  })

  app.post(makePath(requests.modifySub), (req, res) => {
    const {token, username, product, date} = req.body
    if (token === undefined || username === undefined || product === undefined || date === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token.length === 0 || username.length === 0 || product.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    return db.findUser(username)
    .then((user) => {
      return db.findProduct(product)
      .then((productData) => db.modifySubscription(user.id, productData.id, date.begin || productData.date_begin, date.end || productData.date_end))
    })
    .then(() => makeAnswerRaw(res, APICode.Success))
    .catch((err) => makeAnswer(res, err))
  })

  app.post(makePath(requests.delSub), (req, res) => {
    const {token, username, product} = req.body
    if (token === undefined || username === undefined || product === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token.length === 0 || username.length === 0 || product.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    return db.findUser(username)
    .then((user) => {
      return db.findProduct(product)
      .then((productData) => db.removeSubscription(user.id, productData.id))
    })
    .then(() => makeAnswerRaw(res, APICode.Success))
    .catch((err) => makeAnswer(res, err))
  })

  app.post(makePath(requests.delSubs), (req, res) => {
    const {token, username} = req.body
    if (token === undefined || username === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token.length === 0 || username.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    return db.findUser(username)
    .then((user) => db.removeSubscriptions(user.id))
    .then(() => makeAnswerRaw(res, APICode.Success))
    .catch((err) => makeAnswer(res, err))
  })

  app.post(makePath(requests.getUsers), (req, res) => {
    const {token} = req.body
    if (token === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    return db.findUsers()
    .then((users) => makeAnswerRaw(res, APICode.Success, users))
    .catch((err) => makeAnswer(res, err))
  })

  app.post(makePath(requests.getUser), (req, res) => {
    const {token, username} = req.body
    if (token === undefined || username === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token.length === 0 || username.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    return db.findUser(username)
    .then((user) => makeAnswerRaw(res, APICode.Success, user))
    .catch((err) => makeAnswer(res, err))
  })

  app.post(makePath(requests.getProducts), (req, res) => {
    const {token} = req.body
    if (token === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    return db.findProducts()
    .then((products) => makeAnswerRaw(res, APICode.Success, products))
    .catch((err) => makeAnswer(res, err))
  })

  app.post(makePath(requests.getProduct), (req, res) => {
    const {token, product} = req.body
    if (token === undefined || product === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token.length === 0 || product.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    return db.findProduct(product)
    .then((productData) => makeAnswerRaw(res, APICode.Success, productData))
    .catch((err) => makeAnswer(res, err))
  })

  app.post(makePath(requests.getSubscriptions), (req, res) => {
    const {token} = req.body
    if (token === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }

    const selector = () => {
      if (req.body.username !== undefined) {
        return db.findUser(req.body.username)
        .then((user) => db.findSubscriptions(user.id))
      }
      return db.findAllSubscriptions()
    }

    return selector()
    .then((subs) => makeAnswerRaw(res, APICode.Success, subs))
    .catch((err) => makeAnswer(res, err))
  })

  app.post(makePath(requests.getSubscription), (req, res) => {
    const {token, username, product} = req.body
    if (token === undefined || username === undefined || product === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token.length === 0 || username.length === 0 || product.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token !== webCfg.masterToken) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }

    return db.findUser(username)
    .then((user) => {
      return db.findProduct(product)
      .then((productData) => db.findSubscription(user.id, productData.id))
    })
    .then((sub) => makeAnswerRaw(res, APICode.Success, sub))
    .catch((err) => makeAnswer(res, err))
  })

  app.get(makePath(requests.getMyInfos), (req, res) => {
    if (!req.body) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }

    const {token} = req.body
    if (token === undefined) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }
    if (token.length === 0) {
      return makeAnswerRaw(res, APICode.CorruptedRequest)
    }

    return db.findUserByToken(token)
    .then((user) => {
      if (user.api_access === APILevel.Disabled) {
        return makeAnswerRaw(res, APICode.NoApiAccess)
      }
      makeAnswerRaw(res, APICode.Success, {
        username: user.username,
        date_added: user.date_added
      })
    })
    .catch((err) => makeAnswer(res, err))
  })
}
