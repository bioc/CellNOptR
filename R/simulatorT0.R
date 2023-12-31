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
simulatorT0<-function(CNOlist,model,simList,indexList){

    if (!is(CNOlist,"CNOlist")){
        CNOlist = CellNOptR::CNOlist(CNOlist)
    }

    # This simulator is dedicated to the time0 case.
    # It creates a new structure called CNOlistT0, which is a subset of a CNOlist
    # It also sets the valueInhibitors and valueStimuli to zero.
    mi = CNOlist@inhibitors
    ms = CNOlist@stimuli
    # only inhibitors and stimuli are required but need to convert this into a
    # a valid CNOlist class, so we must provide all values

    CNOlistT0 = CNOlist
    CNOlistT0@inhibitors = matrix(1, dim(mi)[1], dim(mi)[2])
    CNOlistT0@stimuli    = matrix(0, dim(ms)[1], dim(ms)[2])

    # !! hack. SimulatorT1 expect the inhibitors values to be 0 or 1.
    # Then, it flips them. So, the 0-values are flipped to 1. finally,
    # the values that are equal to 1 are set to NA...
    # Here, since this is T0, all inhibitors are set to 0. simulatorT1 will
    # therefore flip them to 1 and finally NA. So, we set all inhibitors to 1.

    # Need to be very careful if simulatorT1 changes

    # Finally run the simulator with the particular set of experiments at t0
    newInput = simulatorT1(CNOlistT0, model, simList, indexList, mode=0)
    return(newInput)
    }

