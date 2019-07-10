# license-system

A proof of concept of a license system written in C++ and node.js. Communication is encrypted using a [sealed box](https://libsodium.gitbook.io/doc/public-key_cryptography/sealed_boxes).
The keys for the responses are regenerated with each request. Thus only a known public key exists in the client.

The only known vulnerability of the PoC is the HWID itself. The plain HWID is stored in the database instead of being generated and stored from several components and a user-specific salt, and obv. the used "hwid" itself.

For more informations, checkout the other README files.

## Credits

- @nlohmann (JSON header)
- @jedisct1 ([libsodium](https://github.com/jedisct1/libsodium), [libsodium.js](https://github.com/jedisct1/libsodiumjs))
- @inxilpro ([app-root-path](https://github.com/inxilpro/node-app-root-path))
- @petkaantonov ([bluebird](http://bluebirdjs.com/))
- @expressjs ([express](https://github.com/expressjs/express))
- @jprichardson ([fs-extra](https://github.com/jprichardson/node-fs-extra))
- @moment ([moment](https://github.com/moment/moment))
