// SPDX-License-Identifier: LGPL-3.0-or-later


class SyncedCounter {
    
           //set some original mean and n for the data set
        int mean;
        int n;

        public SyncedCounter(int m, int num){
            this.mean = m;
            this.n = num;
        }

        public void addData(int newData){
        //add new data (100) to the data set
        int newMean = 0;
        int addData = 1;
        try{    
            newMean = (mean + newData)/(n + 1);
            System.out.println("Waiting...");
            while(true){
                
            }
        }catch(ArithmeticException e){
            System.out.println("Woops!ArithmeticException caught. Discarding faulting data");
            addData = 0;
        }

        if(addData == 1){
            mean = newMean;
            n = n+1;
        }

        System.out.print("Adding new data finished. New mean is : ");
        System.out.println(mean);
    }
}

public class MultiThreadMain {
    public static void main(String[] args) throws InterruptedException {
        System.out.println("Starting");
        SyncedCounter data = new SyncedCounter(100,10);
        data.addData(10);

    }
}

