/**
 * @author    ReactiioN
 * @copyright 2019, reactiion.net
 * @license   MIT
 */
'use strict'

const APICode = {
  Success:                   0,
  CorruptedRequest:          1,
  CryptoBoxSealFailed:       2,
  CryptoBoxSealOpenFailed:   3,
  UserAlreadyExists:         4,
  UserNotFound:              5,
  UserIsBanned:              6,
  UserHwidMismatch:          7,
  UserHasNoSubscriptions:    8,
  ProductAlreadyExists:      9,
  ProductNotFound:           10,
  ProductNotActive:          11,
  ProductHasNoFileName:      12,
  ProductHasNoFile:          13,
  SubscriptionAlreadyExists: 14,
  FailedToCheckPath:         15,
  FailedToCreatePath:        16,
  FailedToReadFile:          17,
  FailedToWriteFile:         18,
  NoApiAccess:               19,
  InvalidToken:              20,
  SQL_NotConnected:          21,
  SQL_FailedToExecuteQuery:  22,
  SQL_NullRows:              23
}

APICode.toString = (id) => {
  switch (id) {
    case APICode.Success:                   return 'Success'
    case APICode.CorruptedRequest:          return 'Corrupted request'
    case APICode.CryptoBoxSealFailed:       return 'Failed to use crypto_box_seal'
    case APICode.CryptoBoxSealOpenFailed:   return 'Failed to use crypto_box_seal_open'
    case APICode.UserAlreadyExists:         return 'User already exists'
    case APICode.UserNotFound:              return 'User not found'
    case APICode.UserIsBanned:              return 'User is banned'
    case APICode.UserHwidMismatch:          return 'Hardware Id mismatch'
    case APICode.UserHasNoSubscriptions:    return 'User has no (active) subscription'
    case APICode.ProductAlreadyExists:      return 'Product already exists'
    case APICode.ProductNotFound:           return 'Product not found'
    case APICode.ProductNotActive:          return 'Product not active'
    case APICode.ProductHasNoFileName:      return 'Product has no filename'
    case APICode.ProductHasNoFile:          return 'Product file missing'
    case APICode.SubscriptionAlreadyExists: return 'Subscription already exists for this product'
    case APICode.FailedToCheckPath:         return 'Failed to check path'
    case APICode.FailedToCreatePath:        return 'Failed to create path'
    case APICode.FailedToReadFile:          return 'Failed to read file'
    case APICode.FailedToWriteFile:         return 'Failed to write file'
    case APICode.NoApiAccess:               return 'Forbidden API access'
    case APICode.InvalidToken:              return 'Invalid API token'
    case APICode.SQL_NotConnected:          return 'Missing connection to the mysql server'
    case APICode.SQL_FailedToExecuteQuery:  return 'Failed to execute query'
    case APICode.SQL_NullRows:              return 'Empty rows fetched from database'
    default:                                return `Invalid API code: ${id}`
  }
}

APICode.reject = (id) => {return {
  code: id,
  msg:  APICode.toString(id)
}}

const APILevel = {
  Disabled: 0,
  User:     1,
  Admin:    2
}

APILevel.toString = (id) => {
  switch (id) {
    case APILevel.Disabled: return 'Disabled'
    case APILevel.User:     return 'User'
    case APILevel.Admin:    return 'Admin'
    default:                return `Invalid API level: ${id}`
  }
}

const ClientRequest = {
  Login:      0,
  GetSubs:    1,
  GetProduct: 2,
}

ClientRequest.toString = (id) => {
  switch (id) {
    case ClientRequest.Login:      return 'Login'
    case ClientRequest.GetSubs:    return 'Get Subscriptions"'
    case ClientRequest.GetProduct: return 'Get Product'
    default:                       return `Invalid client request: ${id}`
  }
}

module.exports = {APICode, APILevel, ClientRequest}
