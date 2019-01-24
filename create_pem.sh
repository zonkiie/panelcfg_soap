#!/bin/bash

# https://help.ubuntu.com/community/OpenSSL

OUTDIR=/tmp/sslkey
export SERVER_CONF=${OUTDIR}/server.cnf
export CA_CONF=${OUTDIR}/ca.cnf
PEM_PASSWORD="changeme"
CA_PASSWORD="cachangeme"
mkdir -p ${OUTDIR}
CADIR=${OUTDIR}/myCA
mkdir -p ${CADIR} ${CADIR}/private ${CADIR}/signedcerts

echo -en "
\033[0;31m
================================= WARNING =================================
This creation routine is insecure as the password is in this file and can be seen by other processes, because Password is passed as parameter.
Use this Creation Routine only in development environment!

\033[0m
"

echo "01" > ${CADIR}/serial
touch ${CADIR}/index.txt

cat << ENDOFCACONF > ${CA_CONF}
# My sample caconfig.cnf file.
#
# Default configuration to use when one is not provided on the command line.
#
[ ca ]
default_ca      = local_ca
#
#
# Default location of directories and files needed to generate certificates.
#
[ local_ca ]
dir             = ${CADIR}
certificate     = \$dir/cacert.pem
database        = \$dir/index.txt
new_certs_dir   = \$dir/signedcerts
private_key     = \$dir/private/cakey.pem
serial          = \$dir/serial
#       
#
# Default expiration and encryption policies for certificates.
#
default_crl_days        = 365
default_days            = 1825
default_md              = sha1
#       
policy          = local_ca_policy
x509_extensions = local_ca_extensions
#
#
# Copy extensions specified in the certificate request
#
copy_extensions = copy
#       
#
# Default policy to use when generating server certificates.  The following
# fields must be defined in the server certificate.
#
[ local_ca_policy ]
commonName              = supplied
stateOrProvinceName     = supplied
countryName             = supplied
emailAddress            = supplied
organizationName        = supplied
organizationalUnitName  = supplied
#       
#
# x509 extensions to use when generating server certificates.
#
[ local_ca_extensions ]
basicConstraints        = CA:false
#       
#
# The default root certificate generation policy.
#
[ req ]
default_bits    = 2048
default_keyfile = ${CADIR}/private/cakey.pem
default_md      = sha1
#       
prompt                  = no
distinguished_name      = root_ca_distinguished_name
x509_extensions         = root_ca_extensions
#
#
# Root Certificate Authority distinguished name.  Change these fields to match
# your local environment!
#
[ root_ca_distinguished_name ]
commonName              = MyOwn Root Certificate Authority
stateOrProvinceName     = NC
countryName             = US
emailAddress            = root@tradeshowhell.com
organizationName        = Trade Show Hell
organizationalUnitName  = IT Department
#       
[ root_ca_extensions ]
basicConstraints        = CA:true

ENDOFCACONF

cat << ENDOFSERVERCNF > ${SERVER_CONF}
#
# exampleserver.cnf
#

[ req ]
prompt                  = no
distinguished_name      = debian_server
req_extensions          = v3_req

[ debian_server ]
commonName              = zonkiie.github.com
stateOrProvinceName     = BY
countryName             = DE
emailAddress            = root@zonkiie.github.com
organizationName        = Private
organizationalUnitName  = No Subunit

[ v3_req ]
basicConstraints        = CA:FALSE
keyUsage                = nonRepudiation, digitalSignature, keyEncipherment
#subjectAltName          = @alt_names

#[ alt_names ]
#DNS.0                   = zonkiie.github.com
#DNS.1                   = alt.zonkiie.github.com

ENDOFSERVERCNF

export OPENSSL_CONF=${CA_CONF}

openssl req -x509 -passout pass:${CA_PASSWORD} -newkey rsa:2048 -out ${OUTDIR}/cacert.pem -outform PEM -days 1825
openssl x509 -in ${OUTDIR}/cacert.pem -out ${OUTDIR}/cacert.crt

export OPENSSL_CONF=${SERVER_CONF}

openssl req --passin pass:${CA_PASSWORD} --passout pass:${PEM_PASSWORD} -newkey rsa:1024 -keyout ${OUTDIR}/tempkey.pem -keyform PEM -out ${OUTDIR}/tempreq.pem -outform PEM
openssl rsa --passin pass:${PEM_PASSWORD} < ${OUTDIR}/tempkey.pem > ${OUTDIR}/server_key.pem

#rm -v ${CA_CONF} ${SERVER_CONF}
unset OPENSSL_CONF
