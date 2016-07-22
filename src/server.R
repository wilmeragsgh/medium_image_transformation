library('Rcpp')

#system("export PKG_LIBS=`pkg-config --libs opencv` `R -e 'Rcpp:::LdFlags()' -fopenmp -lgomp`")
#system("export PKG_CFLAGS=`pkg-config --cflags opencv`")
#system("export PKG_CXXFLAGS=`pkg-config --cflags opencv` `R -e 'Rcpp:::CxxFlags()' -fopenmp`")
#system("R CMD SHLIB ../build/negativeT.cpp")
#system("R CMD SHLIB ../build/mirrorTV.cpp")
#system("R CMD SHLIB ../build/mirrorTH.cpp")
#system("R CMD SHLIB ../build/rotateT.cpp")
cat('Wait for it...')
setwd('..')
dyn.load('build/negativeT.so')
dyn.load('build/mirrorTV.so')
dyn.load('build/mirrorTH.so')
dyn.load('build/rotateT.so')
setwd('src')
#Sys.setenv("PKG_LIBS" ="`pkg-config --libs opencv` `Rscript -e 'Rcpp:::LdFlags()' -fopenmp -lgomp`")
#Sys.setenv("PKG_CFLAGS" ="`pkg-config --cflags opencv`")
#Sys.setenv("PKG_CXXFLAGS"="`pkg-config --cflags opencv` `Rscript -e 'Rcpp:::CxxFlags()' -fopenmp`")

#cat('Wait for it...')
#sourceCpp('../build/negativeT.cpp')
#sourceCpp('../build/mirrorTV.cpp')
#sourceCpp('../build/mirrorTH.cpp')
#sourceCpp('../build/rotateT.cpp')
cat('it\'s alive!')
#

options(shiny.maxRequestSize=40*1024^2)
e1 <- new.env(parent = .GlobalEnv)
shinyServer(function(input, output) {

# reading image:
    output$files <- renderTable(input$files)
    
    files <- reactive({
        files <- input$files
        arc <- file(files$datapath,'rb')
        # creating the table:
        headr <- c()
        readBin(arc,what = integer(),size = 2)
        headr <- cbind(headr,filesize = readBin(arc,what = integer(),size = 4))
        readBin(arc,what = integer(),size = 4) #skipping reserved
        readBin(arc,what = integer(),size = 4) #skipping offset 
        readBin(arc,what = integer(),size = 4) #skipping headersize
        headr <- cbind(headr,width = readBin(arc,what = integer(),size = 4))
        headr <- cbind(headr,height = readBin(arc,what = integer(),size = 4))
        headr <- cbind(headr,colourPlanes = readBin(arc,what = integer(),size = 2))
        headr <- cbind(headr,bitsPerPixel = readBin(arc,what = integer(),size = 2))
        headr <- cbind(headr,compressionType = readBin(arc,what = integer(),size = 4))
        headr <- cbind(headr,imageSize = readBin(arc,what = integer(),size = 4))
        headr <- cbind(headr,xResolution = readBin(arc,what = integer(),size = 4))
        headr <- cbind(headr,yResolution = readBin(arc,what = integer(),size = 4))
        headr <- cbind(headr,nColors = readBin(arc,what = integer(),size = 4))
        headr <- cbind(headr,impColors = readBin(arc,what = integer(),size = 4))
        close(arc)
        output$hdr <- renderTable(as.data.frame(headr))
        #files$negativeTransfCount <- input$negativeT
        #files$mirrorHTransfCount <- input$mirrorH
        #files$mirrorVTransfCount <- input$mirrorV
        #files$rotateTransCount <- input$degrees
        files
    })
    
      output$images <- renderUI({
        if(is.null(input$files)) return(NULL)
        #if(input$reloadInput > 0){
        #    image_output_list <- list(imageOutput('image1'))
        #    do.call(tagList, image_output_list)  
        #} else
        #    if(input$negativeT > 0){
        #        image_output_list <- list(imageOutput('image1'))
        #        do.call(tagList, image_output_list)  
        #    } else
          image_output_list <- list(imageOutput('image1'))
          do.call(tagList, image_output_list)
      })
      
      observe({
          if(is.null(input$files)) return(NULL)
          assign('currentImage',value = files()$datapath,envir = e1)
          local({
              output[['image1']] <- 
                  renderImage({
                      list(src = files()$datapath,
                           alt = "Image failed to render")
                  }, deleteFile = F)
          })
      })

# reload image:      
      observe({
          if(input$reloadInput == 0 || is.null(input$files)) return(NULL)
          assign('currentImage',value = files()$datapath,envir = e1)
          local({
              #
              output[['image1']] <- 
                  renderImage({
                      list(src = files()$datapath,
                           alt = "Image failed to render")
                  }, deleteFile = F)
          })
      })
#/      
# processing transformation:
# image negative:
      observe({
          if(input$negativeT == 0 || is.null(input$files)) return(NULL)
          route <- negativeTransformation(get('currentImage',envir = e1))
          assign('currentImage',value = route,envir = e1)
          local({
              #print(route)
              output[['image1']] <- 
                  renderImage({
                      list(src = route,
                           alt = "Image failed to render")
                  }, deleteFile = F)
          })
      })
#/
# image mirrorV:
      observe({
          if(input$mirrorV == 0 || is.null(input$files)) return(NULL)
          route <- mirrorTransformationV(get('currentImage',envir = e1))
          assign('currentImage',value = route,envir = e1)
          local({
              #print(route)
              output[['image1']] <- 
                  renderImage({
                      list(src = route,
                           alt = "Image failed to render")
                  }, deleteFile = F)
          })
      })
#/
# image mirrorH:
      observe({
          if(input$mirrorH == 0 || is.null(input$files)) return(NULL)
          route <- mirrorTransformationH(get('currentImage',envir = e1))
          assign('currentImage',value = route,envir = e1)
          local({
              #print(route)
              output[['image1']] <- 
                  renderImage({
                      list(src = route,
                           alt = "Image failed to render")
                  }, deleteFile = F)
          })
      })
#/
# image rotate:
      observe({
          if(input$degrees == 0 || is.null(input$files)) return(NULL)
          dg <- input$degrees
          trueDegree <- ifelse(dg <0,dg + 360,dg)
          nSteps <- trueDegree/90
          route <- rotateTransformation(get('currentImage',envir = e1),nSteps)
          assign('currentImage',value = route,envir = e1)
          local({
              #print(route)
              output[['image1']] <- 
                  renderImage({
                      list(src = route,
                           alt = "Image failed to render")
                  }, deleteFile = F)
          })
      })
#/
# downloader:
      observe({
          if(input$exportImage == 0) return(NULL)
          local({
              comm <- paste('cp ',get('currentImage',envir = e1),' ./output.bmp',sep = '')
              system(command = comm)
          })
      })
#/
    })