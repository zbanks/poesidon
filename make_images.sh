#!/bin/bash

cd konami
rm out/*
python tetamen_text_add.py
cd ..
echo "Generating code..."
python python_scripts/imgconv.py images/*png konami/title.opng konami/out/*.opng 
