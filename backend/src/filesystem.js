/**
 * @author    ReactiioN
 * @copyright 2019, reactiion.net
 * @license   MIT
 */
'use strict'

const Promise   = require('bluebird'),
      root      = require('app-root-path'),
      fs        = require('fs-extra'),
      {APICode} = require('./enum')

module.exports = {
  pathExists: (filename) => {
    return new Promise((resolve, reject) => {
      fs.pathExists(root.resolve(filename), (err, exists) => {
        if (err) {
          return reject(APICode.reject(APICode.FailedToCheckPath))
        }
        resolve(exists)
      })
    })
  },
  mkdirp: (path) => {
    return new Promise((resolve, reject) => {
      fs.mkdirp(root.resolve(path), (err) => {
        if (err) {
          return reject(APICode.reject(APICode.FailedToCreatePath))
        }
        resolve()
      })
    })
  },
  readFile: (filename) => {
    return new Promise((resolve, reject) => {
      fs.readFile(root.resolve(filename), 'utf8', (err, data) => {
        if (err) {
          return reject(APICode.reject(APICode.FailedToReadFile))
        }
        resolve(data)
      })
    })
  },
  readJson: (filename) => {
    return new Promise((resolve, reject) => {
      fs.readJson(root.resolve(filename), (err, obj) => {
        if (err) {
          return reject(APICode.reject(APICode.FailedToReadFile))
        }
        resolve(obj)
      })
    })
  },
  writeFile: (filename, contents) => {
    return new Promise((resolve, reject) => {
      fs.writeFile(root.resolve(filename), contents, 'utf8', (err) => {
        if (err) {
          return reject(APICode.reject(APICode.FailedToWriteFile))
        }
        resolve()
      })
    })
  },
  writeJson: (filename, contents) => {
    return new Promise((resolve, reject) => {
      fs.writeJson(root.resolve(filename), contents, {spaces: 2, EOL: '\n'}, (err) => {
        if (err) {
          return reject(APICode.reject(APICode.FailedToWriteFile))
        }
        resolve()
      })
    })
  }
}
