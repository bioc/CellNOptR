#
#  This file is part of the CNO software
#
#  Copyright (c) 2011-2012 - EMBL - European Bioinformatics Institute
#
#  File author(s): CNO developers (cno-dev@ebi.ac.uk)
#
#  Distributed under the GPLv3 License.
#  See accompanying file LICENSE.txt or copy at
#      http://www.gnu.org/licenses/gpl-3.0.html
#
#  CNO website: http://www.cellnopt.org
#
##############################################################################
# $Id$

cutAndPlotResultsTN <-function(CNOlist, model,bStrings, plotPDF=FALSE,
    tag=NULL, plotParams=list(maxrow=10))
{

 if (!is(CNOlist,"CNOlist")){
        CNOlist = CellNOptR::CNOlist(CNOlist)
    } 




  tPt=CNOlist@timepoints[1:length(bStrings)+1]
  simList = prep4sim(model)
  indexList = indexFinder(CNOlist, model)

    if ("maxrow" %in% names(plotParams) == FALSE){
        plotParams$maxrow = 10
    }   



    modelCut <- cutModel(model, bStrings[[1]])
    simListCut <- cutSimList(simList, bStrings[[1]])

    # t0
    Sim0 <- simulatorT0(CNOlist=CNOlist,model=modelCut,simList=simListCut,indexList=indexList)
    simResT0 <- as.matrix(Sim0[,indexList$signals, drop=FALSE])
    #simResT0 = Sim0

    # simulate
    simResults<-list()
    simResults[[1]]<-simResT0


    for(i in 1:length(bStrings)){
      cutRes = simulateTN(CNOlist, model, bStrings[1:i])
      cutRes<-cutRes[,indexList$signals]
      simResults[[i+1]]<-cutRes
    }


    mse = plotOptimResultsPan(
      simResults=simResults,
      CNOlist=CNOlist,
      formalism="ssN",
      tPt=tPt,
      plotParams=plotParams
      #timePoints=length(tPt)
    )

  if(plotPDF == TRUE) {
    if(is.null(tag)==TRUE) {
      filename <- paste("SimResultsTN", ".pdf", sep="")
    }
    else {
      filename<-paste(tag, "SimResultsTN.pdf", sep="_")
    }

    mse = plotOptimResultsPan(
      simResults=simResults,
      CNOlist=CNOlist,
      formalism="ssN",
      tPt=tPt,
      pdfFileName=filename,
      pdf=TRUE,
        plotParams=plotParams
      #TimePoints=length(tPt)
    )
  }

  outputFileNames = list()
  return(list(mse=mse, filenames=outputFileNames, simResults=simResults))


}
