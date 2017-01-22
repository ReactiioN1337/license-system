## THIS IS JUST A PROOF OF CONCEPT, ONLY FOR LEARNING PURPOSE!

### Requirements
- An UNIX based distribution (tested on Ubuntu 16.04 x64)
- Webserver (Apache2/nginx)
- PHP >= 5.3
- MySQL >= 5.6
- CMake >= 3.2
- gcc/g++ (tested with 5.4.0)
- Visual Studio 2015 (with update 3)

### Installation
The installation and configuration of the webserver is your homework. Afterwards install the essential software:
```
sudo apt-get install -y build-essential g++ gcc git libcurl4-openssl-dev libcrypto++-dev libssl-dev openssl
```
Create a new MySQL database and add this table:
```
CREATE TABLE userlist(
  id Int(24) auto_increment,
  username TEXT not null,
  password TEXT not null,
  banned Int(1) default 0,
  fail_counter Int(1) default 0,
  cpu TEXT not null,
  computer_name TEXT not null,
  physical_hdd_serial TEXT not null,
  hwid_hash TEXT not null,
  PRIMARY KEY(id)
)
```
I'm storing the folders of all subdomains and the main page in `/var/www/reactiion.pw/`. This folder will be used
for the git repository. The test page will point to `/var/www/reactiion.pw/license_system/web`.

```
# go to the target location where the repo is located at
cd /var/www/reactiion.pw

# cloe the repository using git
git clone https://github.com/ReactiioN1337/license-system

# otherwise PHP can not execute the encoder later :--(
chmod 755 license-system/

# navigate into the client folder
cd license-system/client

# build the encoder
./unix.sh
```
Now we need a RSA public/private key pair. I prefere a `8192` bit key, but `4048` or `1024` should be enough for this.
```
openssl genrsa -out private.pem 8192 && openssl rsa -in private.pem  -pubout -outform DER | openssl base64 -A > pubkey.b64 && openssl  pkcs8 -nocrypt -in private.pem -inform PEM -topk8 -outform DER | openssl base64 -A > seckey.b64 && rm private.pem
```
At least you have to fill out the `/license-system/web/Config.php` file. The content of the `pubkey.b64` will be used inside the client.

### Credits
- @nlohmann (special thanks to him for his json header)
- @jarrow2783 (special thanks to him for his cxxopts header)
- @c42f (special thanks to him for his tinyformat header)
- @frk1
- scumbucket(rifk) (https://high-minded.net/threads/hwids-time-savvy-method.55401/)
- xperroni (http://stackoverflow.com/a/17708801)
