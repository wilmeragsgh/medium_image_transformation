ui <- shinyUI(
    fluidPage(
        tags$head(
            tags$style(
                HTML("
                    @import url('//fonts.googleapis.com/css?family=Lobster|Cabin:400,700');
                    .shiny-text-output {
                      background-color:#fff;
                    }
                    .table.data{
                    width: 100%;
                    }
                    #image1{
                     max-height: 540px;
                     height: 100% !important;
                     width: 100%;
                    }
                    #loadHist{
                     margin-top: 4%;
                    }
                    #reloadHist{
                     margin-top: 4%;
                    }")
                )
            ),
        h1(span("Basic Image Transformation", 
            style = "font-weight: 300; font-family: 'Lobster', cursive;"
            ), 
            style = "font-family: 'Source Sans Pro';
                color: #fff; text-align: center;
                background-image: url('texturebg.png');
                padding: 1%"
           ),
        fluidRow(
            column(
                       width=3,
                       style = 'align: center; text-align: center',
                       wellPanel(fileInput(inputId = 'files', 
                           label = 'Select an Image',
                           accept=c('image/bmp', 'image/bmp','.bmp')
                           ),
                       actionButton('reloadInput',
                                    'Reload image',
                                    icon = icon('refresh'))),
                       hr(),
                       h4('Transformations',style = 'font-weight: 700; text-align: center'),
                       wellPanel(h5('Negative:',
                                               style = 'font-weight: 700; text-align: center'
                                            ),
                                            actionButton(
                                                'negativeT',
                                                'Go!',
                                                icon = icon('adjust')
                                            ),
                                            hr(),
                                            sliderInput(
                                                "degrees", 
                                                "Rotation:",
                                                min=-270, 
                                                max=270, 
                                                value=0,
                                                step = 90
                                            ),
                                            hr(),
                                            sliderInput(
                                                "scalev", 
                                                "Scaling ver:",
                                                min=0, 
                                                max=270, 
                                                value=0,
                                                step = 90
                                            ),
                                            hr(),
                                            sliderInput(
                                                "scaleh", 
                                                "Scaling hor:",
                                                min=0, 
                                                max=270, 
                                                value=0,
                                                step = 90
                                            ),
                                            hr(),
                                            h5('Mirroring:',
                                               style = 'font-weight: 700; text-align: center'
                                            ),
                                            fluidRow(
                                                actionButton('mirrorV',
                                                             'V',
                                                             icon = icon('arrows-v')
                                                ),
                                                actionButton('mirrorH',
                                                             'H',
                                                             icon = icon('arrows-h')
                                                )
                                            ),
                                            hr(),
                                            h5('Equalization:',
                                               style = 'font-weight: 700; text-align: center'
                                            ),
                                            actionButton(
                                                'equalizationB',
                                                'Go!',
                                                icon = icon('bars')
                                            ),
                                            hr(),
                                             sliderInput("slider2",label = 'Umbralization:' ,min = 0, max = 255, 
                                                         value = c(0, 255)),
                                             actionButton(
                                             'umbralizationI',
                                             'Go!',
                                             icon = icon('bars')
                                             ),
                                             hr(),
                                            sliderInput(
                                                "bright", 
                                                "Brightness:",
                                                min=-255, 
                                                max=255, 
                                                value=0,
                                                step = 1
                                 )
                                 ),
                       hr(),
                       h6('Save to ./output.bmp:',
                          style = 'font-weight: 700; text-align: center'
                       ),
                           actionButton('exportImage',
                                      '',
                                      icon = icon('archive'),width = "30%")
                   ),
                   column(width = 9, 
                          tabsetPanel(
                              tabPanel('Image',
                                       uiOutput('images', 
                                                style = 'overflow:auto'),
                                       hr(),
                                       tableOutput('hdr')),
                              tabPanel('Zoom',
                                       fluidRow(column(width = 6,
                                                       plotOutput("plot2", height = 100,
                                                                  brush = brushOpts(
                                                                      id = "plot2_brush",
                                                                      resetOnNew = TRUE)
                                                                  )
                                                       ),
                                                column(width = 6,
                                                       plotOutput("plot3", height = 100)
                                                      )
                                                )
                                       ),
                              tabPanel('Histogram',
                                       column(width = 3,offset = 5,
                                              conditionalPanel('input.loadHist == 0',
                                                               actionButton('loadHist',
                                                                            'Load histogram',
                                                                            icon = icon('bar-chart'))),
                                              conditionalPanel('input.loadHist > 0',
                                                               actionButton('reloadHist',
                                                                            'Reload histogram',
                                                                            icon = icon('refresh'))),
                                              hr()
                                              ),
                                       plotOutput("hist0"),
                                       fluidRow(column(width = 4,plotOutput("hist1", height = 300)),column(width = 4,plotOutput("hist2", height = 300)),column(width = 4,plotOutput("hist3", height = 300))))
                              )
                          )
            )
    )
)