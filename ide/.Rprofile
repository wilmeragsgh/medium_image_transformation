# Remember to put it into '~/'
# or associate a .Rproject where the dir is

## Create a new invisible environment for all the functions to go in so it doesn't clutter your workspace.
.env <- new.env()

## Returns a logical vector TRUE for elements of X not in Y
.env$"%nin%" <- function(x, y) !(x %in% y) 

.env$lib <- function(pkg,with.devtools = F,devtools.source = 'github', ...){
    # Descipcion:
    #   Descarga e instala/carga paquetes segun sea necesario.
    #
    # Args:
    #   pkg: paquete(s) a cargar.
    #   from: fuente del paquete referido a opciones de devtools, ejemplo: github.
    #
    # Retorna:
    #   Carga pkg en el env actual.
    installed.pkgs <- installed.packages()
    is.installed <- pkg %in% installed.pkgs[,1]
    if(!with.devtools){
        if (!is.installed) {
            install.packages(pkg, ...)
            is.installed <- pkg %in% installed.pkgs[,1]
            if (is.installed) stop(paste("load failure:", pkg))
        }
    }
    if(with.devtools){
        if(length(grep(x = ls('package:devtools'),pattern = devtools.source))== 0){
            stop(paste('devtools source not found:',devtools.source))
        }else
            pkg.only <- last(unlist(strsplit(pkg,split = '/'))) # example pkg <- 'vqv/ggbiplot'
        if (!is.installed) {
            do.call(paste('install_',devtools.source,sep = ''),args = list(pkg))
            is.installed <- pkg %in% installed.pkgs[,1]
            if (is.installed) stop(paste("load failure:", pkg))
        }
    }
    library(pkg,character.only = T)
}

## Attach all the variables above
attach(.env)

## .First() run at the start of every R session. 
## Use to load commonly used packages? 
.First <- function() {
	# library(ggplot2)
	cat("\nSuccessfully loaded .Rprofile at", date(), "\n")
        cat('...It means there is cool stuff at ls(.env) \n')
        setwd('../src')
}

## .Last() run at the end of the session
.Last <- function() {
	# save command history here?
	cat("\nGoodbye at ", date(), "\n")
}
