#include <map>
#include <nlohmann/json.hpp>
#include <vector>

enum class EventType {
    USER_CREATED,
    USER_UPDATED,
    USER_DELETED,

    BOOK_CREATED,
    BOOK_UPDATED,
    BOOK_DELETED,
    
    BORROW_RECORD_CREATED,
    BORROW_RECORD_UPDATED,
    BORROW_RECORD_DELETED,

    FILE_CHANGED,
    DATA_RELOADED,

    USER_LOGGED_IN,
    USER_LOGIN_FAILED,
    USER_LOGGED_OUT
};

struct Event {
    EventType type;
    nlohmann::json payload;
};

class EventBus {
   private:
    std::map<EventType, std::vector<std::function<void(const Event&)>>> listeners;

   public:
    static EventBus& getInstance();

    void subscribe(EventType type, std::function<void(const Event&)> callback);

    void publish(const Event& event);
};
