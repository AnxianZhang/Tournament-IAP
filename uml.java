public class LivraisonSA{
    Public List<Ville> villes;
    Private List<Vehicule> camions ;

    public LivraisonSA(){
        this.villes = new ArrayList<>();
        this.camions = new ArrayList<>();
    }
}

public class abstract Vehicule {;
    private String immat;
    public List<Passage> villes;
    
    public Vehicule(String imma){
        this.immat = imma;
        this.villes = new ArrayList<>();
    }
} 


public class Passage{
    private Date datePassage;
    public Vehicules vehicule;
    public Ville ville;

}

public class Vehicule {

    public String getDescription();
}

public class Voiture extends Vehicule {
    
    public String getDescription () {
        System.out.println("Voiture" + super.getImma() + "avec" + getNbPortes() )
    }
}

public class Moto extends Vehicule{
    public String getDescription(){
        System.out.println("Moto " + super.getImma() + " de " + this.getCylindree() )
    }
}

