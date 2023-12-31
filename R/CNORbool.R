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

#' @export
CNORbool<-function(CNOlist, model, paramsList=defaultParameters(),
    compression=TRUE, expansion=TRUE, cutNONC=TRUE, verbose=FALSE,
    timeIndices=NULL)
{


	if (!is(CNOlist,"CNOlist")){
        cnolist = CellNOptR::CNOlist(CNOlist)
    }else{
        cnolist = CNOlist
    }
    if (is.character(model)==TRUE){
        model = readSIF(model)
    }

    # overwrite verbosity
    paramsList$verbose = verbose

    # 1. Checks data to model compatibility
    checkSignals(cnolist, model)
    # 2.preprocessing
    model = preprocessing(cnolist, model, compression=compression,
        expansion=expansion, cutNONC=cutNONC, verbose=verbose)

    #8.Optimisation t1
    bStrings = list()
    initBstring<-rep(1,length(model$reacID))
    print("Entering gaBinaryT1")


    if (is.null(timeIndices)==TRUE){
        T0index = 2
    } else{
        T0index = timeIndices[[1]]
    }

    allRes = c()
    T1opt<-gaBinaryT1(CNOlist=cnolist,
        model=model,
        initBstring=initBstring,
        sizeFac=paramsList$sizeFac,
        NAFac=paramsList$NAFac,
        popSize=paramsList$popSize,
        pMutation=paramsList$pMutation,
        maxTime=paramsList$maxTime,
        maxGens=paramsList$maxGens,
        stallGenMax=paramsList$stallGenMax,
        selPress=paramsList$selPress,
        elitism=paramsList$elitism,
        relTol=paramsList$relTol,
        verbose=paramsList$verbose, 
        timeIndex=T0index)
    bStrings[[1]] = T1opt$bString
    allRes = c(allRes, T1opt)


    if (length(cnolist@signals)==2){
        return(list(model=model, bStrings=bStrings, results=allRes))
    }

    #.Optimise tN where N>1
    Times = 1
    T2opt<-NA # default value

    if (is.null(timeIndices)==TRUE){
        for (i in 3:length(cnolist@signals)){
            Times = Times + 1
            print(paste("Entering gaBinaryTN (time=", Times, ")", sep=" "))

            TNopt<-gaBinaryTN(
                CNOlist=cnolist,
                model=model,
                bStrings=bStrings,
                sizeFac=paramsList$sizeFac,
                NAFac=paramsList$NAFac,
                popSize=paramsList$popSize,
                pMutation=paramsList$pMutation,
                maxTime=paramsList$maxTime,
                maxGens=paramsList$maxGens,
                stallGenMax=paramsList$stallGenMax,
                selPress=paramsList$selPress,
                elitism=paramsList$elitism,
                relTol=paramsList$relTol,
                verbose=paramsList$verbose)

            allRes = c(allRes, TNopt)
            bStrings[[Times]] = TNopt$bString
            print(TNopt$bString)

            if (Times==2){
                T2opt = TNopt
            }
        }
    }else{
        for (i in 2:length(timeIndices)){
            Times = Times + 1
            print(paste("Entering gaBinaryTN (time=", Times, ")", sep=" "))

            TNopt<-gaBinaryTN(
                CNOlist=cnolist,
                model=model,
                bStrings=bStrings,
                sizeFac=paramsList$sizeFac,
                NAFac=paramsList$NAFac,
                popSize=paramsList$popSize,
                pMutation=paramsList$pMutation,
                maxTime=paramsList$maxTime,
                maxGens=paramsList$maxGens,
                stallGenMax=paramsList$stallGenMax,
                selPress=paramsList$selPress,
                elitism=paramsList$elitism,
                relTol=paramsList$relTol,
                verbose=paramsList$verbose, timeIndex=timeIndices[[i]])

            allRes = c(allRes, TNopt)
            bStrings[[Times]] = TNopt$bString
            print(TNopt$bString)

            if (Times==2){
                T2opt = TNopt
            }

        }
    }

    return(list(model=model, bStrings=bStrings, results=allRes))
}
