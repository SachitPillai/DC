import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;

public class rmiserver {
    public static void main(String[] args) {
        try {
            // Create an instance of the remote object
            Hello obj = new HelloImpl();

            // Start the RMI registry on the default port (1099)
            LocateRegistry.createRegistry(1099);

            // Bind the remote object to the registry
            Naming.rebind("Hello", obj);

            System.out.println("server ready.");
        } catch (Exception e) {
            System.err.println("Server exception: " + e.toString());
            e.printStackTrace();
        }
    }
}