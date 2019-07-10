/**
 * @author    ReactiioN
 * @copyright 2019, reactiion.net
 * @license   MIT
 */
'use strict'

{
  const {Log, LogType} = require('node-psr-log')
  global.log = new Log(null, LogType.Everything)
  global.log.setChannel('license-system')
}

const loadConfig = require('./src/config')
const Database   = require('./src/database')

loadConfig()
.then((cfg) => {
  const db = new Database(cfg.mysql)
  db.initialize()
  .then(() => {
    require('./src/server')(cfg, db)
  })
  .catch((err) => console.log(err))
})
