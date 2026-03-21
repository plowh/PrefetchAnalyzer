# PrefetchAnalyzer

A lightweight tool for analyzing Windows Prefetch files.

## Overview

Windows Prefetch files are created by the operating system to optimize application startup. They can also provide useful insight into program execution and system activity.

`PrefetchAnalyzer` is a simple tool for parsing and extracting information from `.pf` files in a clear and accessible way.

## Features

- Parse Windows Prefetch (`.pf`) files  
- Extract execution metadata  
- Check if files are signed, unsigned, or deleted 
- Lightweight and easy to use  

## Disclaimer

This project was built to explore how parsing Prefetch files works and to implement a straightforward parser from scratch. While there are more advanced tools available, this focuses on simplicity and clarity.

## Installation

```bash
git clone https://github.com/plowh/PrefetchAnalyzer.git
cd PrefetchAnalyzer
