
/**
 * @author    ReactiioN
 * @copyright 2019, reactiion.net
 * @license   MIT
 */
'use strict'

module.exports = (config, db)  => {
  const express    = require('express'),
        bodyParser = require('body-parser'),
        app        = express()

  app.use(bodyParser.urlencoded({extended: true}))
  app.use(bodyParser.json())

  app.set('title', config.web.title)
  // https://www.cloudflare.com/ips/
  app.set('trust proxy', [
    'loopback',
    'linklocal',
    'uniquelocal',
    '173.245.48.0/20',
    '103.21.244.0/22',
    '103.22.200.0/22',
    '103.31.4.0/22',
    '141.101.64.0/18',
    '108.162.192.0/18',
    '190.93.240.0/20',
    '188.114.96.0/20',
    '197.234.240.0/22',
    '198.41.128.0/17',
    '162.158.0.0/15',
    '104.16.0.0/12',
    '172.64.0.0/13',
    '131.0.72.0/22'
  ])

  const backend = require('./backend')(app, config, db)

  app.get('/version', (req, res) => backend.writeResponse(res, config.web.version))
  app.get('/online',  (req, res) => backend.writeResponse(res, config.web.active === true ? '1' : '0'))
  app.post('/client', (req, res) => backend.handleClientPostRequest(req, res))

  // page is necessary, so we send forbidden status
  app.get('/', (req, res) => res.sendStatus(403))
  // redirect everything (except for our rest api) to index, which is the forbidden page.
  app.get('*', (req, res) => res.redirect('/'))

  const server = app.listen(config.web.port, () => global.log.notice(`listening at port: ${server.address().port}`))
}
