#!/bin/bash

set -e #Exit immediately if a command exits with a non-zero status.

mongosh << EOF
use $MONGO_INITDB_DATABASE
db.data.createIndex( { device_name : 1 });
db.data.createIndex( { timestamp : 1 });
EOF

user=$(<$MONGO_USER_USERNAME_FILE)
pwd=$(<$MONGO_USER_PASSWORD_FILE)
mongosh << EOF
use admin
db.createUser({
  user: '$user',
  pwd:  '$pwd',
  roles: [{
    role: 'read',
    db: '$MONGO_INITDB_DATABASE'
  }]
})
EOF

user=$(<$MONGO_GATHERER_USERNAME_FILE)
pwd=$(<$MONGO_GATHERER_PASSWORD_FILE)
mongosh << EOF
use admin
db.createUser({
  user: '$user',
  pwd:  '$pwd',
  roles: [{
    role: 'readWrite',
    db: '$MONGO_INITDB_DATABASE'
  }]
})
EOF