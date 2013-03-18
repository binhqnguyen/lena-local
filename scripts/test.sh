TIMEINT=101
                                        if [ $TIMEINT>50 ]; then
                                                 let TIMEINT=TIMEINT+5
                                        else
                                                 if [ $TIMEINT>100 ]; then
                                                           let TIMEINT=TIMEINT+10
                                                 else
                                                           let TIMEINT=TIMEINT+1
                                                 fi
                                        fi