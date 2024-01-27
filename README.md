# stl-to-desmos
converts an stl to desmos art. 
Forked from https://github.com/WagyuDEV/desmos3d but written in C and faster!
Previously it took a few hours to do a 400-500mb file, this takes 17 seconds 😄.

Todo:
- ~~Don't require sed in future releases~~ DONE!
- ~~Command line options~~ DONE!
- MOAR SPEED!

## Speed benchmarks
File used for benchmark was generated using the python script. This file generated is 348351444 bytes, or 333 megabytes.

Version 1 (Commit 3e3d64baca2183ee361e7d79ec67d62c0626952a): 

|         | Mean   | Std.Dev. | Min    | Median | Max    |
|---------|--------|----------|--------|--------|--------|
| real    | 16.846 | 0.170    | 16.678 | 16.779 | 17.079 |
| user    | 6.772  | 0.189    | 6.552  | 6.753  | 7.013  |
| sys     | 2.182  | 0.095    | 2.051  | 2.219  | 2.276  |


Version 1.5 (Unreleased, see V2)
|         | Mean   | Std.Dev. | Min    | Median | Max    |
|---------|--------|----------|--------|--------|--------|
| real    | 6.938  | 0.155    | 6.772  | 6.898  | 7.145  |
| user    | 5.868  | 0.038    | 5.840  | 5.843  | 5.923  |
| sys     | 0.637  | 0.086    | 0.570  | 0.583  | 0.758  |

Version 2
|         | Mean   | Std.Dev. | Min    | Median | Max    |
|---------|--------|----------|--------|--------|--------|
| real    | 6.733  | 0.108    | 6.602  | 6.731  | 6.867  |
| user    | 5.754  | 0.015    | 5.740  | 5.748  | 5.774  |
| sys     | 0.600  | 0.029    | 0.566  | 0.596  | 0.636  |
