#pragma once

#include "../models/BorrowRecord.h"
#include "../repos/JsonRepo.h"

#include <iostream>
#include <vector>
#include <functional>

class BorrowRecordService {
   private:
    JsonRepo<BorrowRecord>& repo;

   public:
    BorrowRecordService(JsonRepo<BorrowRecord>&);

    void addBorrowRecord(BorrowRecord);
    const BorrowRecord* findBorrowRecord(const BorrowRecord&) const;

    template<typename Predicate>
    const std::vector<BorrowRecord> getBorrowedRecordsIf(Predicate pred)const{
        std::vector<BorrowRecord> result;
        for (const auto& book : repo.getAll()) {
            if (pred(book)) {
                result.push_back(book);
            }
        }
        return result;
    };
    const std::vector<BorrowRecord> getOwingRecords() const;
    // const BorrowRecord* findBorrowRecord(const std::string&) const;
    void updateBorrowRecord(const BorrowRecord&) const;
    void returnBook(const std::string& isbn, const std::string& userId) const;
    void deleteBorrowRecord(const BorrowRecord&) const;
    void listBorrowRecords();
};
