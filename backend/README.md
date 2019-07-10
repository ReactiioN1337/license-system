# license-system-backend

The original project was definitely only a **PoC**, but not a very good one. In the last few years I learned a lot about backend development and refactored up the project.
I kicked PHP out of the project because I prefer to work with node.js.

## Features

- Management of users, products, subscriptions with a very lightweight REST api.
- User validation based on hardware id
  - ⚠️ In the project the hardware id is determined via [GetVolumeInformation](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationa). This is not a reliable way and also, the HWID is not hashed on the server.
- Very easy to extend

## Requirements

The whole backend has been tested with the following operation systems:

- macOS Mojave
- Windows 10 1803, 1809
- Ubuntu Server 18.04

Dependency | tested with
:---: | :---:
[node.js](https://nodejs.org/en/)|12.6
MySQL Server | 5.6, 5.7
[nginx](http://nginx.org/en/download.html) | 1.17.1
[yarn](https://yarnpkg.com/lang/en/) | 1.14.0
[Visual Studio 2017](https://docs.microsoft.com/en-gb/visualstudio/releasenotes/vs2017-relnotes) | Latest Version

## Installation

1. Clone the repository recursive (`git clone --recursive https://github.com/ReactiioN1337/license-system`)
2. Navigate to the `license-system` directory
3. Execute the `submodules.sh`
4. Navigate to the `backend` directory
5. Install all dependencies using `yarn`
6. Start the backend once to generate your config file (`node .`)
7. Setup your config file
8. Run the backend permanently using `pm2 start index.js --name backend`
9. Navigate to the `client` directory
10. Open the solution using `Visual Studio 2017`
11. Do some changes related to user stuff
12. Compile it
13. ????
14. Profit.

## nginx configuration

Create a new `.conf` file inside `nginx/conf.d` and save it with the following contents and restart the webserver afterwards.

```nginx
server {
    listen 443 ssl http2;
    listen [::]:443 ssl http2;
    server_name YOURDOMAIN.com;

    location / {
        proxy_pass http://localhost:8001;
        proxy_http_version 1.1;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-Proto $scheme;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    }
}
```

## REST API

I've implemented a very lightweight REST API to manage the backend. Every request uses `application/json` as body and mostly every request is `POST` request.
You should use [Postman](https://www.getpostman.com/) and import the `license-system.postman_collection.json`. Create a new environment variable called `hostname` and use `localhost:8001` or your real URL.

The response contains always `code` and sometimes `response`. Example:

```JSON
{
    "code": 0,
    "response": {"some_data": "yes"}
}
```

### addUser

Adds a new user to the database if the wanted name is available. The first user in the list is given "admin" rights and is not banned. All other users do not have access to the API user method(s) by default and are banned.

**body**

- `name`, the wanted username as `string`
- `token`, the master token as `string`

```JSON
{
    "name": "test-user",
    "token": "9qcefgdr7xjbcmvb"
}
```

### delUser

Removes a user from the database as well as all existing subscriptions.

**body**

- `name`, the associated username as `string`
- `token`, the master token as `string`

```JSON
{
    "name": "test-user",
    "token": "9qcefgdr7xjbcmvb"
}
```

### banUser

Bans an user temp. or permanently.

**body**

- `name`, the associated username as `string`
- `token`, the master token as `string`
- `date`, the date as `number` (UNIX timestamp) or `-1` (permanent)

```JSON
{
  "token": "9qcefgdr7xjbcmvb",
  "name": "test-user",
  "date": -1
}
```

### resetHwid

Deletes the current HWID for a specific user.

**body**

- `name`, the associated username as `string`
- `token`, the master token as `string`

```JSON
{
    "name": "test-user",
    "token": "9qcefgdr7xjbcmvb"
}
```

### setApiAccess

Sets the API access for an user.

**body**

- `name`, the associated username as `string`
- `token`, the master token as `string`
- `level`, the wanted access level as `number`
  - `0` - Disabled
  - `1` - User
  - `2` - Admin

```JSON
{
  "token": "9qcefgdr7xjbcmvb",
  "name": "ReactiioN",
  "level": 3
}
```

### addProduct

Adds a new product to the database if the name is available.

**body**

- `active`, optional and by default `false`
- `file_name`, optional and by default empty

```JSON
{
  "name": "test-product",
  "token": "9qcefgdr7xjbcmvb",
  "active": true,
  "file_name": "my_file.bin"
}
```

### delProduct

Deletes a product from the database as well as all existing subscriptions.

**body**

- `name`, the associated product name as `string`
- `token`, the master token as `string`

```JSON
{
  "name": "test-product",
  "token": "9qcefgdr7xjbcmvb"
}
```

### modifyProduct

Editing product informations.

**body**

- `name`, the associated product name as `string`
- `token`, the master token as `string`,
- `active`, optional as `boolean`
- `file`, optional as `object`
  - `name`, the filename as `string`
  - `contents`, the contents as `string`

```JSON
{
  "name": "test-product",
  "token": "9qcefgdr7xjbcmvb",
  "active": false,
  "file": {
    "name": "another_file_name.bin",
    "contents": "this should be the file contents as base64"
  }
}
```

### addSubscription

Adds a new subscription for an user.

**body**

- `token`, the master token as `string`,
- `username`, the associated username as `string`
- `product`, the associated product name as `string`,
- `date`, optional as `object`
  - `begin`, the begin date as `number` (UNIX timestamp) or -1 (lifetime).
  - `end`, the end date as `number` (UNIX timestamp) or -1 (lifetime).

```JSON
{
  "token": "9qcefgdr7xjbcmvb",
  "username": "test-user",
  "product": "test-product",
  "date": {
    "begin": -1,
    "end": -1
  }
}
```

### modifySubscription

Modifies an user-specified subscription.

**body**

- `token`, the master token as `string`,
- `username`, the associated username as `string`
- `product`, the associated product name as `string`,
- `date`, optional as `object`
  - `begin`, the begin date as `number` (UNIX timestamp) or -1 (lifetime).
  - `end`, the end date as `number` (UNIX timestamp) or -1 (lifetime).

```JSON
{
  "token": "9qcefgdr7xjbcmvb",
  "username": "test-user",
  "product": "test-product",
  "date": {
    "begin": -1,
    "end": -1
  }
}
```

### delSubscription

Removes an user-specific subscription.

**body**

- `token`, the master token as `string`,
- `username`, the associated username as `string`
- `product`, the associated product name as `string`

```JSON
{
  "token": "9qcefgdr7xjbcmvb",
  "username": "test-user",
  "product": "test-product"
}
```

### delSubscriptions

Removes all user subscriptions.

**body**

- `token`, the master token as `string`,
- `username`, the associated username as `string`

```JSON
{
  "token": "9qcefgdr7xjbcmvb",
  "username": "test-user",
  "product": "test-product"
}
```

### getUsers

Gets a list of all users and their informations.

**body**

- `token`, the master token as `string`

```JSON
{
  "token": "9qcefgdr7xjbcmvb"
}
```

**response**

```JSON
{
  "code": 0,
  "msg": "Success",
  "response": [
    {
      "id": 1,
      "username": "test-user",
      "hwid_hash": "",
      "date_added": 1562170319,
      "ban_date": 0,
      "api_access": 3,
      "api_token": "3ffdcaiori1ahkgc"
    },
    {
      "id": 2,
      "username": "test-user2",
      "hwid_hash": "",
      "date_added": 1562237976,
      "ban_date": -1,
      "api_access": 0,
      "api_token": "6a0gwytqjlz57b36"
    }
  ]
}
```

### getUser

Get an user with all informations.

**body**

- `token`, the master token as `string`,
- `username`, the associated username as `string`

```JSON
{
  "token": "9qcefgdr7xjbcmvb",
  "username": "test-user"
}
```

**response**

```JSON
{
  "code": 0,
  "msg": "Success",
  "response": {
    "id": 1,
    "username": "test-user",
    "hwid_hash": "",
    "date_added": 1562170319,
    "ban_date": 0,
    "api_access": 3,
    "api_token": "3ffdcaiori1ahkgc"
  }
}
```

### getProducts

Gets a list of all products and their informations.

**body**

- `token`, the master token as `string`

```JSON
{
  "token": "9qcefgdr7xjbcmvb"
}

**response**

```JSON
{
  "code": 0,
  "msg": "Success",
  "response": [
    {
      "id": 1,
      "name": "test-product",
      "active": 1,
      "file_name": "test-product.bin"
    }
  ]
}
```

### getProduct

Get a product with all informations.

**body**

- `token`, the master token as `string`,
- `product`, the associated product name as `string`

```JSON
{
  "token": "9qcefgdr7xjbcmvb",
  "product": "test-product"
}
```

**response**

```JSON
{
  "code": 0,
  "msg": "Success",
  "response": {
    "id": 1,
    "name": "test-product",
    "active": 1,
    "file_name": "test-product.bin"
  }
}
```

### getSubscription

Gets all user-specified subscriptions.

**body**

- `token`, the master token as `string`,
- `username`, the associated username as `string`
- `product`, the associated product name as `string`

```JSON
{
  "token": "9qcefgdr7xjbcmvb",
  "username": "test-user",
  "product": "test-product"
}
```

**response**

```JSON
{
  "code": 0,
  "msg": "Success",
  "response": {
    "id": 1,
    "user_id": 1,
    "product_id": 1,
    "date_begin": -1,
    "date_end": -1
  }
}
```

### getSubscriptions

Gets every product subscriptions or every user-specified product subscriptions.

**body**

- `token`, the master token as `string`,
- `product`, the associated product name as `string`,
- `username`, *optional* the associated username as `string`

```JSON
{
  "token": "9qcefgdr7xjbcmvb",
  "username": "test-user",
  "product": "test-product"
}

{
  "token": "9qcefgdr7xjbcmvb",
  "product": "test-product"
}
```

**response**

```JSON
{
  "code": 0,
  "msg": "Success",
  "response": [
    {
      "id": 1,
      "user_id": 1,
      "product_id": 1,
      "date_begin": -1,
      "date_end": -1
    }
  ]
}

{
  "code": 0,
  "msg": "Success",
  "response": [
    {
      "id": 1,
      "user_id": 1,
      "product_id": 1,
      "date_begin": -1,
      "date_end": -1
    },
    {
      "id": 2,
      "user_id": 2,
      "product_id": 1,
      "date_begin": -1,
      "date_end": -1
    }
  ]
}
```

### getMyInformations

Gets the correct username and date added. This is a user-friendly `GET`-request method.

**body**

- `token`, the user-specified API token as `string`

```JSON
{
	"token": "3ffdcaiori1ahkgc"
}
```

**response**

```JSON
{
  "code": 0,
  "msg": "Success",
  "response": {
    "username": "test-user",
    "date_added": 1562170319
  }
}
```
