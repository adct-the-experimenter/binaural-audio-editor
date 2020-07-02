#!/usr/bin/env bash

if [ -d '${HOME}/binaural-audio-editor/resources']
then
	echo "data directoy is in ${HOME}/binaural-audio-editor/resources."
else
	echo "creating data directory in home directory..."
	mkdir -p ${HOME}/binaural-audio-editor/resources
