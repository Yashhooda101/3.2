import jakarta.persistence.*;

@Entity
@Table(name = "Student")
public class Student {
    @Id
    private int id;

    private String name;
    private int age;

    // Constructors
    public Student() {}
    public Student(int id, String name, int age) {
        this.id = id;
        this.name = name;
        this.age = age;
    }

    // Getters and setters
    public int getId() { return id; }
    public void setId(int id) { this.id = id; }

    public String getName() { return name; }
    public void setName(String name) { this.name = name; }

    public int getAge() { return age; }
    public void setAge(int age) { this.age = age; }
}
<hibernate-configuration>
  <session-factory>
    <property name="hibernate.connection.driver_class">com.mysql.cj.jdbc.Driver</property>
    <property name="hibernate.connection.url">jdbc:mysql://localhost:3306/your_database</property>
    <property name="hibernate.connection.username">your_username</property>
    <property name="hibernate.connection.password">your_password</property>
    <property name="hibernate.dialect">org.hibernate.dialect.MySQLDialect</property>
    <property name="hibernate.hbm2ddl.auto">update</property>
    <property name="show_sql">true</property>

    <mapping class="Student"/>
  </session-factory>
</hibernate-configuration>
import org.hibernate.*;
import org.hibernate.cfg.Configuration;

public class StudentDAO {
    private static SessionFactory factory = new Configuration().configure().buildSessionFactory();

    public void createStudent(Student s) {
        Session session = factory.openSession();
        Transaction tx = session.beginTransaction();
        session.save(s);
        tx.commit();
        session.close();
    }

    public Student readStudent(int id) {
        Session session = factory.openSession();
        Student s = session.get(Student.class, id);
        session.close();
        return s;
    }

    public void updateStudent(Student s) {
        Session session = factory.openSession();
        Transaction tx = session.beginTransaction();
        session.update(s);
        tx.commit();
        session.close();
    }

    public void deleteStudent(int id) {
        Session session = factory.openSession();
        Transaction tx = session.beginTransaction();
        Student s = session.get(Student.class, id);
        if (s != null) session.delete(s);
        tx.commit();
        session.close();
    }
}
public class Main {
    public static void main(String[] args) {
        StudentDAO dao = new StudentDAO();

        // Create
        dao.createStudent(new Student(1, "Alice", 20));

        // Read
        Student s = dao.readStudent(1);
        System.out.println(s.getName());

        // Update
        s.setAge(21);
        dao.updateStudent(s);

        // Delete
        dao.deleteStudent(1);
    }
}
