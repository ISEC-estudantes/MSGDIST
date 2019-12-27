#!/bin/bash

ls *.c |
while read fich
do
   echo "Analisar ${fich}..."
   grep $fich Makefile > /dev/null
   if test $? -eq 1
   then
      echo "Nao existe"
      echo $fich >> Makefile
   fi
   sleep 1
done

exit 0
