#! /bin/sh
#
# install.sh
# Copyright (C) 2019 vam <jpwan21@gmail.com>
#
# Distributed under terms of the MIT license.
#

export GOPATH=`pwd`
cd src
go install main
cd -
