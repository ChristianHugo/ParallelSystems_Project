#!/bin/bash

# Skript für automatisierte Zeitmessung der numerischen berechnung der Wellengleichung.
# Die Binaries schreiben auf die Standardausgabe ihre Laufzeit, das Skript liest und parst sie.
#
# Es werden die sequentielle, openMP, std und OpenMPI Varianten aufgerufen.
# Die Größe Der Matrix erhöht sich mit jeden Schritt um 1000 in die Zeit und Positionsrichtung.
# Ein Binary wir 4 mal aufgerufen die Laufzeit gemittelt und ebenfalls auf die Standardausgabe geschrieben.

handleStrat()
{
  # vergrößert die Matrix
  for step in $(seq 1 7)
  do
    nx=$((step*1000))
    nt=$((step*1000))
    runningTimeSum=0

    # 4 mal ausführen und mitteln
    for i in $(seq 1 4)
    do
      runStats=""
      if [[ $1 == "3" ]]; then
        runStats=$(mpiexec -n $2 ./open_mpi_main num_threads=$2 nt=$nt nx=$nx)
      else 
        runStats=$(./main execution_strategy=$1 num_threads=$2 nt=$nt nx=$nx)
      fi
      echo $runStats    
      declare -a 'arr=($runStats)'
      runningTimeSum=$(echo ${arr[4]} + $runningTimeSum  | bc -l)
    done
    average=$(echo $runningTimeSum / 4 | bc -l)
    echo $average
    echo ""


  done
}

# sequentielle
handleStrat 0 1

# über sequentiel, OpenMP und std::thread
for strat in $(seq 1 2)
do
  echo ""
  # threads hochzählen
  for threadCount in 2 4 8
  do
    echo ""
    handleStrat $strat $threadCount
  done
done

# und noch das OpenMPI Binary
echo ""
for threadCount in 2 4
do
  echo ""
  handleStrat "3" $threadCount
done


