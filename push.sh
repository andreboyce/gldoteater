#!/bin/bash

#rm -rf .git
#git init
#git config user.email "mobile@andreboyce.com"
#git config user.name "Andre Boyce"
#git add -A
#git commit -m "first commit"
#git branch -M main
#git remote add origin 'git@github.com:andreboyce/gldoteater.git'
#git remote set-url origin 'git@github.com:andreboyce/gldoteater.git'

MESSAGE=$1

if [ -z "$MESSAGE" ]
then
   MESSAGE="update";
else
   :
fi

git add -A
git commit -m "$MESSAGE"
git push -u origin main
