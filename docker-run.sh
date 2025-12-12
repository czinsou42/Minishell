#!/bin/bash
docker build -t minishell-dev .
docker run -it --rm -v "$(pwd):/minishell" minishell-dev