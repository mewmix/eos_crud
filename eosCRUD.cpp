#include <eosio/eosio.hpp>
#include <eosio/print.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract("addressbook")]] addressbook : public eosio::contract
{
    public:
        addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}

        [[eosio::action]]
        void upsert (name user, std::string time, string temp){
            address_index addresses(get_self(), get_first_receiver().value);

            addresses.emplace(user, [&]( auto& row ) {
      row.key = user;
      row.time = time;
      row.temp = temp;

      print("row.key", row.key);
      print("row.time",row.time);
      print("row.temp",row.temp);

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
            print("Post_user: ", iterator->key, "  Post_time: ", iterator->time.c_str(), " Content: ", iterator->temp.c_str());
        }
    private:
        struct [[eosio::table]] data {
            name key;
            string time;
            string temp;

            uint64_t primary_key() const {return key.value ;}
            EOSLIB_SERIALIZE(data, (key)(time)(temp))
        };

        typedef eosio::multi_index<"aggregate"_n, data> address_index;





};
