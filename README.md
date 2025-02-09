A tool to help the incompetent free software enthusiast code review open source libraries.
It works exactly like your workflow:
### FOSS Enthusiast Workflow:
1: some indie developer posts a lib they spent weeks or months designing, on reddit/dayle.dev<br>
2: the FOSS Enthusiast opens the lib's github in a random file, with 0 context, and 0 understanding of why each thing is there, then omits a bunch of nonsense opinions about the project.

## avgfosshelper workflow:
you call the avgfosshelper cli with:

```bash
avgfosshelper <github-repo-url>
```
It will place a random file with a random chunk of code from that file on your screen, repeat until you have enough to write a blog post about how the project is bad.


## Building
The construction of this project is done using cmake.

To compile you need to go into the build folder and run the following command:
```bash
mkdir build
cd build

cmake ..

make
```
## ClI flags
## Flags
| Flag | Description | Default |
|------|-------------|---------|
| ---cache | The Cache dir |  /tmp/cache |
| --timeout | The timeout to reupdate the url | 300s |
| --line-chunk | The size of the chunk of the random text |40 lines|


## Features
- 4 max chars variables size 
- comments for every action (expecialy the for the obvious)
- low quantity of files 
- minimum of 200 lines por file
- All functions have a short name with abbreviations, just like you would.
- This project does not work for cpp, because it only needs to work for me.
- Underneath there is a mathematical calculation to predict which file you would be most likely to choose.

