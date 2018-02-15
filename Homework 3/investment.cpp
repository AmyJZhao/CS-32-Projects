//abstract base class: investment
class Investment
{
    public:
        Investment(string name, int price)
        {
            m_name = name;
            m_price = price;
        }
        int purchasePrice() const
        {
            return m_price;
        }
        string name() const
        {
            return m_name;
        }
        //pure virtual functions
        virtual bool fungible() const = 0;
        virtual string description() const = 0;
        //virtual destructor
        virtual ~Investment()
        {
            cout << endl;
        }
    private:
        string m_name; //every investment has a name
        int m_price; //every investment has a price
};

//sub class: painting
class Painting : public Investment
{
    public:
        Painting(string name, int price)
        : Investment(name, price)
        {
        
        }
        virtual bool fungible() const
        {
            return false; //a painting is not fungible
        }
        virtual string description() const
        {
            return "painting";
        }
        virtual ~Painting()
        {
            cout << "Destroying " + name() << ", a " << description();
        }
};

//sub class: stock
class Stock : public Investment
{
    public:
        Stock(string name, int price, string ticker)
        : Investment(name, price)
        {
            m_ticker = ticker;
        }
        virtual bool fungible() const
        {
            return true; //a stock is fungible
        }
        virtual string description() const
        {
            return "stock trading as " + m_ticker;
        }
        virtual ~Stock()
        {
            cout << "Destroying " << name() << ", a stock holding";
        }
    private:
        string m_ticker; //a stock also has a ticker symbol
};

//sub class: house
class House : public Investment
{
    public:
        House(string name, int price)
        : Investment(name, price)
        {
            
        }
        virtual bool fungible() const
        {
            return false; //a house is not fungible
        }
        virtual string description() const
        {
            return "house";
        }
        virtual ~House()
        {
            cout << "Destroying the " << description() << " " << name();
        }
};
