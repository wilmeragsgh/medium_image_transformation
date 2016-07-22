# Mid Image Transformations
Shiny app to allow the performing of the following tasks over an image in '.bmp' format:
* Show it's metadata.
* Show it's histogram.
* Ecualize it.
* Apply mirroring effect.
* Transform it to it's negative.
* Modify it's brightness and contrast.
* Umbralization given a threshold.
* Scaling and rotation of the image.
* Nearest and bilinear interpolation.
* Zoom in / out.

### Prerequisities

The following code will install dependencies that ensure you could run this software, if you are aware of equivalent conditions, feel free of trying to run it on them, also you could contribute by sharing your environment with us and it will be very appreciate it. 

#### Install r:
```
#! /bin/bash

sh -c 'echo "deb http://cran.rstudio.com/bin/linux/ubuntu trusty/" >> /etc/ap$
gpg --keyserver keyserver.ubuntu.com --recv-key E084DAB9
gpg -a --export E084DAB9 | sudo apt-key add -
apt-get update
apt-get install -y r-base r-base-dev
```

### Install image libraries:
```
#! /bin/bash

apt-get install libtiff-devel fftw-devel
apt-get install xorg libx11-dev
apt-get install fftw3 
apt-get install libopencv-dev

```

#### Install R libraries:
```
#! /bin/bash

R -e 'install.packages(c('shiny','Rcpp'))'
```
## Deployment

To run the app you just
```
R -e "shiny::runGitHub('pdi_homework-2','wilmeragsgh',subdir = 'src')"
```
Then you should see:
```
Loading required package: shiny

Listening on http://127.0.0.1:PORT

```
But wait for it while compiling
...then Enjoy!

## Built With

* x86_64-pc-linux-gnu (64-bit)
* R version 3.3.0 (2016-05-03) -- "Supposedly Educational"
* RStudio version 0.99.892
* Shiny(R package) version 0.13.2

## Authors

* **Wilmer Gonzalez** - *Initial work*

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Prof. Esmitt Ramirez
