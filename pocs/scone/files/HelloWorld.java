public class HelloWorld {

    public static void main(String[] args) {
        System.out.println("Starting");
        try{
            int x = 0;
            while(true){
                // x = x/x;
                x = x+1;

 	    }

        }catch(ArithmeticException e){
            System.out.println("Woops!ArithmeticException caught");
        }
    }

}
