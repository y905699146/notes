

class Exam2{

    bool IncUserVisitsAndCheckLimit(unsigned int user_id){
        User* user = GetUserByID(user_id);

        if user->limit{
            return false;
        }

        auto curTime = GetCurrentTime();

        user -> termTime.push_back(curTime);
        size_t TermSize = user -> termTime.size();
    

        if (user -> termTime[TermSize-1] - user -> termTime[0] > 60){
    
            user -> termTime.pop_front()

            return true;
        }else {        
            user->VisitCount ++;
            if user->VisitCount >15{
                return false;
            }
            return true;
        }


    }
    User* GetUserByID(unsigned int user_id){
        return UserMap[user_id];
    }
    private:
        std::unorder_map<unsigned int, User*> UserMap;
}

unsigned long GetCurrentTime(){  //返回当前Unix时间
        return time.Now().Unix()
}
struct User{
    bool limit ;
    unsigned int  user_id;
    unsigned int  VisitCount;
    std deque<unsigned int> termTime;
    unsigned long VisitTerm;
    
}


