#!/bin/bash

if [[ -f build/install_manifest.txt ]]
then
	echo "Removing library files..."
	xargs rm < build/install_manifest.txt
fi

echo "Done!"
