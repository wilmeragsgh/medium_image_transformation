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
        sidebarLayout(sidebarPanel(
                       width=3,
                       style = 'align: center; text-align: center',
                       fileInput(inputId = 'files', 
                           label = 'Select an Image',
                           accept=c('image/bmp', 'image/bmp','.bmp')
                           ),
                       actionButton('reloadInput',
                                    'Reload image',
                                    icon = icon('refresh')),
                       hr(),
                       h5('Negative:',
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
                       h6('Save to ./output.bmp:',
                          style = 'font-weight: 700; text-align: center'
                       ),
                           actionButton('exportImage',
                                      '',
                                      icon = icon('archive'),width = "30%")
                   ),
                   mainPanel(uiOutput(
                       'images',
                       style = 'overflow: auto;'
                   ),width = 9
            )),
        hr(),
        tableOutput('hdr')
    )
)