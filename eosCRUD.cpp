#include <eosio/eosio.hpp>
#include <eosio/print.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract("addressbook")]] addressbook : public eosio::contract
{
    public:
        addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}

        [[eosio::action]]
        void upsert (name user, std::string first_name, string c_city){
            address_index addresses(get_self(), get_first_receiver().value);

            addresses.emplace(user, [&]( auto& row ) {
      row.key = user;
      row.fname = first_name;
      row.city = c_city;

      print("row.key", row.key);
      print("row.fname",row.fname);
      print("row.city",row.city);

        });

        }

        [[eosio::action]]
        void rub(name user){
            require_auth(user);
    address_index addresses(get_self(), get_first_receiver().value);
    auto iterator = addresses.find(user.value);
    check(iterator != addresses.end(), "Record does not exist");
    addresses.erase(iterator);

        }

        [[eosio::action]]
        void read(name user){
            require_auth(user);
            address_index addresses(get_self(), get_first_receiver().value);
            auto iterator = addresses.find(user.value);
            check(iterator != addresses.end(), "Record does not exist");
            print("Post_user: ", iterator->key, "  Post_fname: ", iterator->fname.c_str(), " Content: ", iterator->city.c_str());
        }
    private:
        struct [[eosio::table]] person {
            name key;
            string fname;
            string city;

            uint64_t primary_key() const {return key.value ;}
            EOSLIB_SERIALIZE(person, (key)(fname)(city))
        };

        typedef eosio::multi_index<"people"_n, person> address_index;





};
