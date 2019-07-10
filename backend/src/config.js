/**
 * @author    ReactiioN
 * @copyright 2019, reactiion.net
 * @license   MIT
 */
'use strict'

const fs                = require('fs-extra'),
      root              = require('app-root-path'),
      {KeyPair, sodium} = require('./cryptlib'),
      randomString      = () => {return Math.random().toString(36).slice(-8) + Math.random().toString(36).slice(-8)}

module.exports = () => {
  return sodium.ready
  .then(() => fs.pathExists(root.resolve('data')))
  .then((exists) => {
    if (exists !== true) {
      return fs.mkdirp(root.resolve('data'))
    }
  })
  .then(() => fs.readJson(root.resolve('data/settings.json')))
  .catch((err) => {
    if (err.syscall === 'open') {
      const kp  = new KeyPair(),
            obj = {
        web: {
          active:      false,
          version:     "1.0.0",
          title:       '',
          port:        8001,
          masterToken: randomString(),
          requestPath: 'api',
          requests: {
            addUser:          'addUser',
            delUser:          'delUser',
            banUser:          "banUser",
            resetHwid:        "resetHwid",
            setApiAccess:     "setApiAccess",
            addProduct:       "addProduct",
            delProduct:       "delProduct",
            modifyProduct:    "modifyProduct",
            addSub:           "addSubscription",
            modifySub:        "modifySubscription",
            delSub:           "delSubscription",
            delSubs:          "delSubscriptions",
            getUsers:         "getUsers",
            getUser:          "getUser",
            getProducts:      "getProducts",
            getProduct:       "getProduct",
            getSubscriptions: "getSubscriptions",
            getSubscription:  "getSubscription",
            getMyInfos:       "getMyInformations"
          }
        },
        socket: {
          token:      randomString(),
          publicKey:  kp.getPublicKey(),
          privateKey: kp.getPrivateKey()
        },
        mysql: {
          host:            'localhost',
          user:            '',
          password:        '',
          database:        '',
          connectionLimit: 15
        }
      }
      return fs.writeJson(root.resolve('data/settings.json'), obj, {spaces: 2, EOL: '\n'})
      .then(() => global.log.warning('please setup the settings.json first!'))
      .then(() => {process.exit(-1)})
      .catch(() => {
        global.log.warning('failed to write settings.json, process exit..')
        return Promise.reject()
      })
    }
  })
}
