import java.rmi.Naming;

public class rmiclient{
    public static void main(String[] args) {
        try {
            // Lookup the remote object
            Hello stub = (Hello) Naming.lookup("rmi://localhost/Hello");

            // Perform some operations
            System.out.println(stub.sayHello());
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString());
            e.printStackTrace();
        }
    }
}