CellNOptR
=========
<!-- badges: start -->
  [![Lifecycle: stable](https://img.shields.io/badge/lifecycle-stable-brightgreen.svg)](https://lifecycle.r-lib.org/articles/stages.html#stable)
  [![BioC status](http://www.bioconductor.org/shields/build/release/bioc/CellNOptR.svg)](https://bioconductor.org/checkResults/release/bioc-LATEST/CellNOptR)
  <!-- badges: end -->
  

Training of boolean logic models of signalling networks using prior knowledge networks and perturbation data.

- Please visit [CellNOptR](https://saezlab.github.io/CellNOptR/) for details about the project (references, news, ...)


## Installation:

Before starting, make sure you have installed the latest version of R. For more information and download
of R, please refer to [R project page](http://www.r-project.org/) . For more information about how to 
install R packages, please refer to [Installing packages](http://cran.r-project.org/doc/manuals/R-admin.html#Installing-packages).

### Standard installation from Bioconductor
To install CellNOptR, type:
```
if (!requireNamespace("BiocManager", quietly = TRUE))
    install.packages("BiocManager")
BiocManager::install("CellNOptR", version = "3.8")
```

Then, you can also install other CellNOptR related packages::
```
BiocManager::install("CNORdt")
BiocManager::install("CNORfeeder")
BiocManager::install("CNORfuzzy")
BiocManager::install("CNORode")
```


### Installation from GitHub
Using the `devtools` package you can install the latest version from the GitHub repository:
```
if(!require("devtools")) install.packages("devtools")   # installs devtools package if not already installed
devtools::install_github("saezlab/CellNOptR", build_vignettes = TRUE)
```
To see the documentation of the package and a few examples, please type:
```
vignette("CellNOptR-vignette")
```



### Install from a local copy of the package:
install CellNOptR from a tar ball as follows:
```
install.packages("path_to_CellNOptR/CellNOptR_1.0.0.tar.gz", repos=NULL, type="source")
```
or, using the R GUI by clicking on "Packages & Data" then "Package installer", then choosing "local source"
from the dropdown menu, clicking "install", choosing CellNOptR.1.0.0.tar.gz
and finally clicking "open".


## Feedbacks, bug reports, features
Feedbacks and bugreports are always very welcomed!  
Please use the Github issue page to report bugs or for questions: https://github.com/saezlab/CellNOptR/issues.
Many thanks!

## Code of Conduct
  Please note that the CellNOptR project is released with a [Contributor Code of Conduct](http://bioconductor.org/about/code-of-conduct/). By contributing to this project, you agree to abide by its terms.
  
