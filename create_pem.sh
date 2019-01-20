#!/bin/bash

# https://help.ubuntu.com/community/OpenSSL

export OPENSSL_CONF=/tmp/server.cnf
OUTDIR=/tmp/sslkey
PEM_PASSWORD="changeme"
mkdir -p ${OUTDIR}

echo -en "
\033[0;31m
================================= WARNING =================================
This creation routine is insecure as the password is in this file and can be seen by other processes, because Password is passed as parameter.
Use this Creation Routine only in development environment!

\033[0m
"

cat << ENDOFSERVERCNF > ${OPENSSL_CONF}
#
# exampleserver.cnf
#

[ req ]
prompt                  = no
distinguished_name      = server_distinguished_name
req_extensions          = v3_req

[ server_distinguished_name ]
commonName              = tradeshowhell.com
stateOrProvinceName     = NC
countryName             = US
emailAddress            = root@tradeshowhell.com
organizationName        = My Organization Name
organizationalUnitName  = Subunit of My Large Organization

[ v3_req ]
basicConstraints        = CA:FALSE
keyUsage                = nonRepudiation, digitalSignature, keyEncipherment
subjectAltName          = @alt_names

[ alt_names ]
DNS.0                   = tradeshowhell.com
DNS.1                   = alt.tradeshowhell.com
ENDOFSERVERCNF

openssl req -x509 -passout pass:${PEM_PASSWORD} -newkey rsa:2048 -out ${OUTDIR}/cacert.pem -outform PEM -days 1825
openssl x509 -in ${OUTDIR}/cacert.pem -out ${OUTDIR}/cacert.crt
openssl req --passin pass:${PEM_PASSWORD} --passout pass:${PEM_PASSWORD} -newkey rsa:1024 -keyout ${OUTDIR}/tempkey.pem -keyform PEM -out ${OUTDIR}/tempreq.pem -outform PEM
openssl rsa --passin pass:${PEM_PASSWORD} < ${OUTDIR}/tempkey.pem > ${OUTDIR}/server_key.pem

rm -v ${OPENSSL_CONF}
unset OPENSSL_CONF
