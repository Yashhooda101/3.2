import jakarta.persistence.*;

@Entity
public class Account {
    @Id
    private int id;
    private String holderName;
    private double balance;

    // Getters and setters
    public int getId() { return id; }
    public void setId(int id) { this.id = id; }

    public String getHolderName() { return holderName; }
    public void setHolderName(String holderName) { this.holderName = holderName; }

    public double getBalance() { return balance; }
    public void setBalance(double balance) { this.balance = balance; }
}
import jakarta.persistence.*;
import java.util.Date;

@Entity
public class TransactionRecord {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;

    private int fromAccount;
    private int toAccount;
    private double amount;
    private Date timestamp;

    // Getters and setters
    public int getId() { return id; }
    public int getFromAccount() { return fromAccount; }
    public void setFromAccount(int fromAccount) { this.fromAccount = fromAccount; }

    public int getToAccount() { return toAccount; }
    public void setToAccount(int toAccount) { this.toAccount = toAccount; }

    public double getAmount() { return amount; }
    public void setAmount(double amount) { this.amount = amount; }

    public Date getTimestamp() { return timestamp; }
    public void setTimestamp(Date timestamp) { this.timestamp = timestamp; }
}
import jakarta.persistence.EntityManager;
import jakarta.persistence.PersistenceContext;
import org.springframework.stereotype.Repository;

@Repository
public class AccountDAO {
    @PersistenceContext
    private EntityManager em;

    public Account findById(int id) {
        return em.find(Account.class, id);
    }

    public void update(Account account) {
        em.merge(account);
    }

    public void saveTransaction(TransactionRecord tr) {
        em.persist(tr);
    }
}
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.Date;

@Service
public class BankService {
    @Autowired
    private AccountDAO dao;

    @Transactional
    public void transferMoney(int fromId, int toId, double amount) {
        Account from = dao.findById(fromId);
        Account to = dao.findById(toId);

        if (from.getBalance() < amount) {
            throw new RuntimeException("Insufficient funds");
        }

        from.setBalance(from.getBalance() - amount);
        to.setBalance(to.getBalance() + amount);

        dao.update(from);
        dao.update(to);

        TransactionRecord tr = new TransactionRecord();
        tr.setFromAccount(fromId);
        tr.setToAccount(toId);
        tr.setAmount(amount);
        tr.setTimestamp(new Date());

        dao.saveTransaction(tr);
    }
}
import org.springframework.context.annotation.*;
import org.springframework.orm.jpa.*;
import org.springframework.transaction.annotation.EnableTransactionManagement;

import javax.persistence.EntityManagerFactory;
import javax.sql.DataSource;
import org.springframework.jdbc.datasource.DriverManagerDataSource;

@Configuration
@EnableTransactionManagement
@ComponentScan(basePackages = "your.package")
public class AppConfig {

    @Bean
    public DataSource dataSource() {
        DriverManagerDataSource ds = new DriverManagerDataSource();
        ds.setDriverClassName("com.mysql.cj.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/your_database");
        ds.setUsername("your_username");
        ds.setPassword("your_password");
        return ds;
    }

    @Bean
    public LocalContainerEntityManagerFactoryBean entityManagerFactory() {
        LocalContainerEntityManagerFactoryBean emf = new LocalContainerEntityManagerFactoryBean();
        emf.setDataSource(dataSource());
        emf.setPackagesToScan("your.package");
        emf.setJpaVendorAdapter(new HibernateJpaVendorAdapter());
        return emf;
    }

    @Bean
    public JpaTransactionManager transactionManager(EntityManagerFactory emf) {
        return new JpaTransactionManager(emf);
    }
}
