#include "BorrowRecordService.h"

#include <iomanip>
#include <iostream>

using namespace std;
using chronoSysClk = std::chrono::system_clock;

BorrowRecordService::BorrowRecordService(JsonRepo<BorrowRecord>& r) : repo(r) {}

void BorrowRecordService::addBorrowRecord(BorrowRecord br) {
    // BorrowRecord b;
    // b.read();
    repo.add(br);
}

const BorrowRecord* BorrowRecordService::findBorrowRecord(const BorrowRecord& key) const {
    return repo.find(key);  // find with ISBN
}

// const BorrowRecord* BorrowRecordService::findBorrowRecord(const std::string& key) const {
//     const BorrowRecord* res = repo.find(key);  // find with isbn

//     return res;
// }
const std::vector<BorrowRecord> BorrowRecordService::getOwingRecords() const {
    // std::vector<BorrowRecord> owingRecords;
    // auto all = repo.getAll();
    // for (const auto& br : all){
    //     auto now = ChronoSysClk::now();
    //     int days = now - br.getDueDate()
    // }
    return getBorrowedRecordsIf([&](const auto& br) { return br.isOverdue(); });
}
void BorrowRecordService::updateBorrowRecord(const BorrowRecord& b) const { repo.update(b); }

void BorrowRecordService::returnBook(const std::string& isbn, const std::string& userId) const {
    auto records = repo.getAll();
    for (auto& br : records) {
        // cout << "In returnBook********" << endl;
        if (br.getIsbn() == isbn && br.getBorrowerId() == userId && !br.isReturned()) {
            // cout << "Marking returned" << endl;
            br.markReturned();
            repo.update(br);
            return;
        }
    }
}

void BorrowRecordService::deleteBorrowRecord(const BorrowRecord& b) const { repo.remove(b); }

void BorrowRecordService::listBorrowRecords() {
    const auto& borrowRecords = repo.getAll();
    cout << endl
         << left << setw(20) << "ISBN" << setw(40) << "Borrower Id" << setw(20) << "Borrowed Date"
         << setw(20) << "Due Date" << setw(20) << "Returned Date" << endl;
    for (auto& b : borrowRecords) {
        b.display();
        // std::cout << b << std::endl;
    }
}
