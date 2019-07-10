/**
 * @author    ReactiioN
 * @copyright 2019, reactiion.net
 * @license   MIT
 */
'use strict'

const sodium    = require('libsodium-wrappers-sumo'),
      Promise   = require('bluebird'),
      {APICode} = require('./enum')

const notBase64 = /[^A-Z0-9+\/=]/i

const isBase64 = (input) => {
  const len = input.length
    if (len === 0 || len % 4 !== 0 || notBase64.test(input)) {
      return false
    }

    const  firstPaddingChar = input.indexOf('=')
    return firstPaddingChar === -1 ||
      firstPaddingChar === len - 1 ||
      (firstPaddingChar === len - 2 && input[len - 1] === '=')
}

const base64encode = (input)  => {return Buffer.from(input).toString('base64')}
const base64decode = (cipher) => {return Buffer.from(cipher, 'base64')}

class KeyPair
{
  constructor(publicKey = null, privateKey = null)
  {
    this._publicKey  = null
    this._privateKey = null

    if (publicKey !== null && publicKey !== undefined && isBase64(publicKey)) {
      this.loadPublicKey(publicKey)
      // decryption requires a public key, so any other place would be wrong!
      if (privateKey !== null && privateKey !== undefined && isBase64(privateKey)) {
        this.loadPrivateKey(privateKey)
      }
    } else {
      const ref = sodium.crypto_box_keypair()
      this._publicKey  = ref.publicKey
      this._privateKey = ref.privateKey
    }
  }

  loadPublicKey(publicKey)
  {
    this._publicKey = base64decode(publicKey)
    return this
  }

  loadPrivateKey(privateKey)
  {
    this._privateKey = base64decode(privateKey)
    return this
  }

  getPublicKey()
  {
    return base64encode(this._publicKey)
  }

  getPrivateKey()
  {
    return base64encode(this._privateKey)
  }
}

module.exports = {
  base64encode,
  base64decode,
  isBase64,
  KeyPair,
  sodium,
  sealedboxEncrypt: (data, keyPair) => {
    return new Promise((resolve, reject) => {
      try {
        const cipher = base64encode(sodium.crypto_box_seal(data, keyPair._publicKey))
        return resolve(cipher)
      } catch (e) {
        return reject(APICode.reject(APICode.CryptoBoxSealFailed))
      }
    })
  },
  sealedboxDecrypt: (cipher, keyPair) => {
    return new Promise((resolve, reject) => {
      try {
        const buf = Buffer.from(sodium.crypto_box_seal_open(
          base64decode(cipher),
          keyPair._publicKey,
          keyPair._privateKey
        ))
        return resolve(buf.toString())
      } catch (e) {
        return reject(APICode.reject(APICode.CryptoBoxSealOpenFailed))
      }
    })
  }
}
