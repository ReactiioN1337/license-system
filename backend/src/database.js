/**
 * @author    ReactiioN
 * @copyright 2019, reactiion.net
 * @license   MIT
 */
'use strict'

const knack     = require('knack-sql'),
      Promise   = require('bluebird'),
      moment    = require('moment'),
      {APICode} = require('./enum')

module.exports = class Database
{
  constructor(config)
  {
    this._pool        = knack.mysql.createPool(config)
    this._initialized = false
  }

  initialize()
  {
    if (this._initialized === true) {
      return Promise.resolve()
    }

    return this._exec(
      knack.create('userlist')
      .integer('id', 8)
        .auto_increment()
      .string('username', 64)
        .not_null().default('')
      .string('hwid_hash')
        .not_null().default('')
      .integer('date_added', 8)
        .not_null().default(0)
      .integer('ban_date', 8)
        .not_null().default(0)
      .integer('api_access', 1)
        .not_null().default(0)
      .string('api_token', 64)
        .not_null().default('')
    ).then(() => this._exec(
      knack.create('products')
      .integer('id', 8)
        .auto_increment()
      .string('name', 64)
        .not_null().default('')
      .integer('active', 1)
        .not_null().default(0)
      .string('file_name')
        .not_null().default('')
    )).then(() => this._exec(
      knack.create('subscriptions')
      .integer('id', 8)
        .auto_increment()
      .integer('user_id', 8)
        .not_null().default(0) // todo: foreign key
      .integer('product_id', 8) // todo: foreign key
        .not_null().default(0)
      .integer('date_begin', 8)
        .not_null().default(0)
      .integer('date_end', 8)
        .not_null().default(0)
    )).then(() => this._exec(
      knack.create('log')
      .integer('id', 8)
        .auto_increment()
      .integer('date_added', 8)
        .not_null().default(0)
      .string('username', 64)
        .not_null().default('')
      .text('event_log')
    )).then(() => {this._initialized = true})
  }

  _exec(query, numResults = 0)
  {
    return new Promise((resolve, reject) => {
      this._pool.getConnection((err, connection) => {
        if (err) {
          return reject(APICode.reject(APICode.SQL_NotConnected))
        }
        connection.query(typeof(query) === 'string' ? query : query.serialize(), (err, results, fields) => {
          // we don't need the connection anymore and to prevent against some errors in a long term of usage, we're going to destroy the connection.
          connection.destroy()

          if (err) {
            return reject(APICode.reject(APICode.SQL_FailedToExecuteQuery))
          }

          if (numResults !== null && numResults !== undefined && numResults > 0) {
            if (numResults !== results.length) {
              return reject(APICode.reject(APICode.SQL_NullRows))
            }
          }
          return resolve([results, fields])
        })
      })
    })
  }

  findUser(username)
  {
    return this._exec(
      knack.select(
        'id', 'username', 'hwid_hash', 'date_added',
        'ban_date', 'api_access', 'api_token'
      ).from('userlist')
      .where(['username', username])
      .limit(1), 1
    ).then(([rows]) => {return rows[0]})
  }

  findUsers()
  {
    return this._exec(
      knack.select(
        'id', 'username', 'hwid_hash', 'date_added',
        'ban_date', 'api_access', 'api_token'
      ).from('userlist')
    ).then(([rows]) => {
      if (rows.length === 0) {
        return Promise.reject(APICode.reject(APICode.SQL_NullRows))
      }
      return rows
    })
  }

  getUserlistCount()
  {
    return this._exec(
      knack.select('id').from('userlist')
    ).then(([rows]) => {return rows.length})
  }

  findUserByToken(token)
  {
    return this._exec(
      knack.select(
        'id', 'username', 'hwid_hash', 'date_added',
        'ban_date', 'api_access', 'api_token'
      ).from('userlist')
      .where(['api_token', token])
      .limit(1), 1
    ).then(([rows]) => {return rows[0]})
  }

  findProduct(name)
  {
    return this._exec(
      knack.select('id', 'name', 'active', 'file_name')
      .from('products')
      .where(['name', name])
      .limit(1), 1
    ).then(([rows]) => {return rows[0]})
  }

  findProductById(id)
  {
    return this._exec(
      knack.select('id', 'name', 'active', 'file_name')
      .from('products')
      .where(['id', id])
      .limit(1), 1
    ).then(([rows]) => {return rows[0]})
  }

  findProducts()
  {
    return this._exec(
      knack.select('id', 'name', 'active', 'file_name')
      .from('products')
    ).then(([rows]) => {
      if (rows.length === 0) {
        return Promise.reject(APICode.reject(APICode.SQL_NullRows))
      }
      return rows
    })
  }

  findSubscription(userId, productId)
  {
    return this._exec(
      knack.select(
        'id', 'user_id', 'product_id',
        'date_begin', 'date_end'
      ).from('subscriptions')
      .where(
        ['user_id',    userId],
        ['product_id', productId]
      ).limit(1), 1
    ).then(([rows]) => {return rows[0]})
  }

  findSubscriptions(userId)
  {
    return this._exec(
      knack.select(
        'id', 'user_id', 'product_id',
        'date_begin', 'date_end'
      ).from('subscriptions')
      .where(['user_id', userId])
    ).then(([rows]) => {
      if (rows.length === 0) {
        return Promise.reject(APICode.reject(APICode.SQL_NullRows))
      }
      return rows
    })
  }

  findAllSubscriptions()
  {
    return this._exec(
      knack.select(
        'id', 'user_id', 'product_id',
        'date_begin', 'date_end'
      ).from('subscriptions')
    ).then(([rows]) => {
      if (rows.length === 0) {
        return Promise.reject(APICode.reject(APICode.SQL_NullRows))
      }
      return rows
    })
  }

  addUser(username, apiAccess = 0, apiToken = '', banDate = -1)
  {
    return this.findUser(username)
    .then(() => Promise.reject(APICode.reject(APICode.UserAlreadyExists)))
    .catch((err) => {
      if (err.code !== APICode.SQL_NullRows) {
        return Promise.reject(err)
      }
      return this._exec(
        knack.insert('userlist')
        .values(
          ['username',   username],
          ['date_added', moment().unix()],
          ['api_access', apiAccess],
          ['api_token',  apiToken],
          ['ban_date',   banDate] // ban the user by default (-1)
        )
      )
    })
  }

  addProduct(name, active = false, fileName = '')
  {
    return this.findProduct(name)
    .then(() => Promise.reject(APICode.reject(APICode.ProductAlreadyExists)))
    .catch((err) => {
      if (err.code !== APICode.SQL_NullRows) {
        return Promise.reject(err)
      }
      return this._exec(
        knack.insert('products')
        .values(
          ['name',      name],
          ['active',    active === true ? 1 : 0],
          ['file_name', fileName]
        )
      )
    })
  }

  addSubscription(userId, productId, dateBegin = 0, dateEnd = 0)
  {
    return this.findSubscription(userId, productId)
    .then(() => Promise.reject(APICode.reject(APICode.SubscriptionAlreadyExists)))
    .catch((err) => {
      if (err.code !== APICode.SQL_NullRows) {
        return Promise.reject(err)
      }
      return this._exec(
        knack.insert('subscriptions')
        .values(
          ['user_id',    userId],
          ['product_id', productId],
          ['date_begin', dateBegin],
          ['date_end',   dateEnd]
        )
      )
    })
  }

  setUserHwidHash(username, hwidHash)
  {
    return this.findUser(username)
    .then(() => this._exec(
      knack.update('userlist')
      .set(['hwid_hash', hwidHash])
      .where(['username', username])
    ))
  }

  setUserBan(username, banDate)
  {
    return this.findUser(username)
    .then(() => this._exec(
      knack.update('userlist')
      .set(['ban_date', banDate])
      .where(['username', username])
    ))
  }

  setUserApiAccess(username, accessLevel)
  {
    return this.findUser(username)
    .then(() => this._exec(
      knack.update('userlist')
      .set(['api_access', accessLevel])
      .where(['username', username])
    ))
  }

  setUserApiToken(username, token)
  {
    return this.findUser(username)
    .then(() => this._exec(
      knack.update('userlist')
      .set(['api_token', token])
      .where(['username', username])
    ))
  }

  modifySubscription(userId, productId, dateBegin = undefined, dateEnd = undefined)
  {
    this.findSubscription(userId, productId)
    .then((sub) => this._exec(
      knack.update('subscriptions')
      .set(
        ['date_begin', dateBegin || sub.date_begin],
        ['date_end',   dateEnd   || sub.date_end]
      ).where(
        ['user_id',    userId],
        ['product_id', productId]
      )
    ))
  }

  setProductActive(name, active)
  {
    return this.findProduct(name)
    .then(() => this._exec(
      knack.update('products')
      .set(['active', active])
      .where(['name', name])
    ))
  }

  setProductFileName(name, fileName)
  {
    return this.findProduct(name)
    .then(() => this._exec(
      knack.update('products')
      .set(['file_name', fileName])
      .where(['name', name])
    ))
  }

  removeProduct(name)
  {
    return this.findProduct(name)
    .then((product) => {
      return this._exec(
        knack.drop('products')
        .where(['name', name])
      ).then(() => this._exec(
        knack.drop('subscriptions')
        .where(['product_id', product.id])
      ))
    })
  }

  removeSubscription(userId, productId)
  {
    return this.findSubscription(userId, productId)
    .then(() => this._exec(
      knack.drop('subscriptions')
      .where(
        ['user_id',    userId],
        ['product_id', productId]
      )
    ))
  }

  removeSubscriptions(userId)
  {
    return this.findSubscriptions(userId)
    .then(() => this._exec(
      knack.drop('subscriptions')
      .where(['user_id', userId])
    ))
  }

  /**
   * Removes an user (+ subscriptions) from the database.
   * @param {string} username the associated name.
   * @returns Promise<void>
   */
  removeUser(username)
  {
    return this.findUser(username)
    .then((user) => {
      return this._exec(
        knack.drop('userlist')
        .where(['username', username])
      ).then(() => {
        return this.removeSubscriptions(user.id)
        .catch((err) => {
          if (err.code !== APICode.SQL_NullRows) {
            return Promise.reject(err)
          }
          // user has been deleted but had a subscription.
          return Promise.resolve()
        })
      })
    })
  }

  addLog(message, username = '')
  {
    return this._exec(
      knack.insert('log')
      .values(
        ['date_added', moment().unix()],
        ['username',   username],
        ['event_log',  message]
      )
    )
  }
}
