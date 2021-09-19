//
// Created by BedTed on 5/17/2021.
//


#include "DPS.h"

class MyString {
private:
    void newNextBlock(DPS *head) { // вставка в конец DPS
        DPS *newBlock = new DPS();
        newBlock->vector_ = new char[max_];
        head->next_ = newBlock;
        newBlock->last_ = head;
    }

    void newBetweenNextBlock(DPS *head) { // вставка между текущем и следующим
        DPS *newBlock;
        newBlock->vector_ = new char[max_];

        newBlock->next_ = head->next_;   // новый на второй
        head->next_->last_ = newBlock; // второй на новый
        newBlock->last_ = head;        // новый на первый
        head->next_ = newBlock;        // первый на новый

    }

    void newLastBlock(DPS *head) { // вставка в начало DPS
        DPS *newBlock;
        newBlock->vector_ = new char[max_];
        head->last_ = newBlock;
        newBlock->next_ = head;
    }

    void del(DPS *head) { // удаление блока
        if (head != nullptr) {
            if (head->next_ == nullptr) { // конец
                if (head->last_ != nullptr) head->last_->next_ = nullptr;
            } else if (head->last_ == nullptr) { // начало
                head->next_->last_ = nullptr;
            } else { // между
                head->last_->next_ = head->next_;
                head->next_->last_ = head->last_;
            }
            delete (head->vector_);
        }
    }

    void connect (DPS *head, DPS *added) { // вставка блока added после head
        if (head->next_ != nullptr) {
            DPS *help = added;

            while (help->next_ != nullptr) {
                help = help->next_;
            }

            help->next_ = head->next_;
            head->next_->last_ = help;

            head->next_ = added;
            added->last_ = head;
        } else {
            added->next_ = head->next_; // null
            // head->next_->last_ = added;
            head->next_ = added;
            added->last_ = head;
        }
    }

    void clearFreeBlock () {
        DPS *help = dps_;
        while (help != nullptr) {
            help = help->next_;
            if (dps_->now_ == 0 && help != nullptr) del(help);
            if (help != nullptr) dps_ = help;
        }

        help = dps_;
        while (help != nullptr) {
            help = dps_;
            help = help->last_;
        }
    }

public:
    DPS *dps_;
    int max_;

    MyString(int max) {
        max_ = max;
        dps_ = new DPS(max_);
    }

    MyString(const MyString &obj) {
        max_ = obj.max_;
        DPS *head = obj.dps_;
        DPS *this_head;

        if (obj.dps_ != nullptr) {
            dps_ = new DPS(max_);
            this_head = dps_;
            do {

                for (int i = 0; i < head->now_; ++i) {
                    this_head->vector_[i] = head->vector_[i];
                    this_head->now_++;
                }

                head = head->next_;

                if (head != nullptr) {
                    newNextBlock(this_head);
                }

                this_head = this_head->next_;

            } while (head != nullptr);
        }
    }

    ~MyString() {
        if (dps_ != nullptr) {
            DPS *head = this->dps_;
            DPS *next_head;

            do {
                next_head = head->next_;
                head->~DPS();
                head = next_head;
            } while (next_head != nullptr);
        }
        dps_ = nullptr;
    }

    int len() {
        int sum = 0;
        DPS *head = this->dps_;

        do {
            sum += head->now_;
            head = head->next_;
        } while (head != nullptr);
        return sum;
    }

    friend ostream &operator<<(ostream &out, MyString &MySTR) { // вывод
        DPS *head = MySTR.dps_;

        do {
            for (int i = 0; i < head->now_; ++i) { // мб заменить на while и \0
                out << head->vector_[i];
            }
            head = head->next_;
        } while (head != nullptr);
        return out;
    }

    friend istream &operator>>(istream &sin, MyString &MySTR) { // ввод

        MySTR.~MyString();
        MySTR.dps_ = new DPS(MySTR.max_);
        string in_str;
        int count = 0;
        DPS *head = MySTR.dps_;

        getline(cin, in_str);

        do {
            for (int i = 0; i < MySTR.max_; ++i) {
                if (in_str[count] == '\0') {
                    return sin;
                }
                head->vector_[i] = in_str[count];
                head->now_++;
                count++;
            }
            if (in_str.length() != count) MySTR.newNextBlock(head);
            head = head->next_;
        } while (head != nullptr);
        return sin;
    }

    // превращение строки в строки из блоков
    bool Add(string in_str) {

        if (this->dps_ == nullptr) {
            this->dps_ = new DPS(max_);
        }

        int count = 0;
        DPS *head = this->dps_;

        if (this->dps_->now_ != 0 || this->dps_->next_ != nullptr) {
            DPS *help_head = this->dps_;
            while (help_head != nullptr) {
                head = help_head;
                help_head = head->next_;
            }
            newNextBlock(head);
            head = head->next_;
        }

            do { // цикл блоков
                for (int i = 0; i < this->max_; ++i) {
                    if (in_str[count] == '\0') {
                        return true;
                    }
                    head->vector_[i] = in_str[count];
                    head->now_++;
                    count++;
                }
                if (in_str.length() > count) {
                    this->newNextBlock(head);
                }
                head = head->next_;
            } while (head != nullptr);
            return false;


    }

    // перегрузка присваивания
    MyString &operator=(const MyString inStr/* то, что присваиваем */) { //
        /* this это то, чему присваиваем */

        this->~MyString();  // деструктим, чтобы отчистить строку, к котрой присваиваем
        //max_ = inStr.max_;???
        DPS *inStr_head = inStr.dps_; // смотрит на блоки того что присваиваем
        DPS *this_head;     // смотрит на блоки this


        if (inStr.dps_ != nullptr) { // если входная строка не была удалена, то переписываетм в новые блоки строки из inStr
            dps_ = new DPS(max_); // если входная строка не null, то выделяем память под новый блок в this
            this_head = dps_;
            int inStr_count = 0;
            int this_count = 0;

            do {
                if (inStr.dps_->now_ != 0) { // переписыаем символы
                    this_head->vector_[this_count] = inStr_head->vector_[inStr_count];
                    inStr_count++;
                    this_count++;
                    this_head->now_++;
                }

                if (inStr_count == inStr_head->now_) { // переходим на новый блок inStr
                    inStr_count = 0;
                    inStr_head = inStr_head->next_;
                    if (inStr_head == nullptr) break; // inStr закончилась
                }

                if (this_count == max_) { // переходим на новый блок this
                    this_count = 0;
                    newNextBlock(this_head); // создали новый блок в конце
                    this_head = this_head->next_;
                }

            } while (inStr_head != nullptr); // по факту while TRUE !!!
        }

        this_head = nullptr;
        inStr_head = nullptr;
        return *this;
    }

    // перегрузка  DPS + DPS
    friend MyString operator+(MyString a, MyString b) { // на выходе строка с длиной блока как a
        /*
         * Могут прийти блоки разной длины
         * Присоединяем в конец блоки строки из b
         */
        //if (a.len() != 0)
//        a.clearFreeBlock();
//        b.clearFreeBlock();

        MyString sum(a);

        DPS *head = b.dps_; // идёт по b
        DPS *sum_tail = sum.dps_;
        DPS *this_sum = sum.dps_; // идёт по sum

        while (sum_tail->next_ != nullptr) { // доходим до конца sum
            sum_tail = sum_tail->next_;
        }

        if (b.dps_ != nullptr && b.len() != 0) { // переписываетм в новые блоки строки из b

            sum.newNextBlock(sum_tail); // создали новый блок в конеце строки sum
            this_sum = sum_tail->next_;
            int count_b = 0;
            int count_sum = 0;

            do {       // идём по строке sum размерности блока a.max_ = sum.max_
                // и идём по строке b размерности блока b.max_

                this_sum->vector_[count_sum] = head->vector_[count_b];
                count_b++;
                count_sum++;
                this_sum->now_++;

                if (count_b >= head->now_) {
                    count_b = 0;
                    head = head->next_;
                    if (head == nullptr) break;
                }

                if (count_sum == sum.max_) {
                    count_sum = 0;
                    sum.newNextBlock(this_sum); // завязали новый блок на конец строки sum
                    this_sum = this_sum->next_;
                }

            } while (true);
        }

        this_sum = nullptr;
        head = nullptr;
        if (sum.len() != 0 && sum.dps_->now_ == 0) {
            sum.dps_ = sum.dps_->next_;
            if (sum.dps_ != nullptr) {
                sum.dps_->last_ = nullptr;
            }
        }

        return sum;
    }

    // перегрузка  string + DPS
    friend MyString operator+(string a, MyString b) {
        MyString sum(b.max_); // создали мою строку
        sum.Add(a); // добавили туда строку
        return sum + b; // соеденили
    }

    // перегрузка  DPS + string
    friend MyString operator+(MyString a, string b) {
        MyString sum(a.max_);// создали мою строку
        sum.Add(b); // добавили туда строку
        return a + sum; // соеденили
    }

    // Вроде работает
    // отсчёт с нуля. first - первый символ, count - кол-во символов для копирования, max - кол-во символов в получаемом блоке
    MyString copy(unsigned int first, unsigned int count, unsigned int max) {
        MyString myCopy(max);           // создаём контейнер для копии
        DPS *obj_h = dps_;              // смотрит на блоки this
        DPS *new_h = myCopy.dps_;       // смотрит на блоки myCopy

        if (this->len() >= first + count /*&& count > 0*/) { // если длина строки больше или равна первому символу + кол-ву
            int count_obj = 0;
            int count_new = 0;

            while (count_obj + obj_h->now_ < first) { // идём до блока с first (первым элементом)
                    count_obj += obj_h->now_;
                    obj_h = obj_h->next_;
            }
            // count_obj смотрит на первый элемент нужного блока

            int index = 0;
            int new_index = 0;

            while (count_obj < first) { // идём по блоку с первым элементом
                count_obj++;
                index++;
            } // получили индекс first

            // переписываем в контейнер для копирования
            while (count_new < count) { // пока кол-во переписаного меньше чем кол-во символов для копирования

                if (new_h->now_ >= myCopy.max_){
                    newNextBlock(new_h);
                    new_h = new_h->next_;
                    new_index = 0;
                }

                if (index >= obj_h->now_) {
                    obj_h = obj_h->next_;
                    index = 0;
                }
                if (obj_h == nullptr) return myCopy;
                new_h->vector_[new_index] = obj_h->vector_[index];

                new_h->now_++;
                index++;
                new_index++;
                count_new++;
            }

            return myCopy;
        }
        return myCopy;
    }

    // проверить
    int search(MyString &obj) {
        DPS *str = dps_;
        DPS *helpstr = dps_;
        DPS *key = obj.dps_;
        int keyLen = obj.len();
        int thisLen = this->len();
        int count = 0;
        int count_key = 0;
        int j = 0;
        int i = 0;
        int help_i = 0;
        if (thisLen == 0 || keyLen == 0 || keyLen > thisLen) return -1;

        do {
            j = 0;
            count_key = 0;
            key = obj.dps_;
            helpstr = str;
            help_i = i;
            while (count_key < keyLen && helpstr->vector_[help_i] == key->vector_[j]) {
                j++;
                help_i++;
                count_key++;
                if (j >= key->now_){
                    key = key->next_;
                    j = 0;
                }
                if (help_i >= helpstr->now_) {
                    helpstr = helpstr->next_;
                    help_i = 0;
                }
            }
            if (count_key == keyLen) return count;
            i++;
            count++;
            if (i >= str->now_) {
                str = str->next_;
                i = 0;
            }
        } while (count <= thisLen - keyLen && str != nullptr);
        return -1;
    }

    // проверить (тут должен быть пиздец)
    void searchReplase (MyString &search /* то что ищем */, MyString &replase /* то на что меням  */) {
        DPS *Left; // левая часть строки (в момент разрезания)
        DPS *Right; // правая часть строки (в момент разрезания)
        DPS *helpL; // защита от nullptr
        DPS *helpForDel; // помощь для удаления блоков
//        DPS *copy_help;

        int cutIndex = this->search(search);
        while (cutIndex != -1) {
            MyString cutL(max_);
            MyString cutR(max_);

            MyString copyReplace (max_);
            copyReplace = replase.copy(0,  replase.len(), max_); // создание копии строки replace

            Left = dps_; // левый укахатель смотрит на начало исходной строки

            int countCharToSearch = 0; // кол-во символов от начала до искомого (счётчик)
            while (countCharToSearch + Left->now_ - 1 < cutIndex) {
                countCharToSearch += Left->now_;
                Left = Left->next_;
            } // countCharToSearch смотрит на первый элемент нужного блока DPS

            int index = 0; // индекс начала в первом блоке подстроки
            int end_index = 0; // индекс конца в последенм блоке подстроки
            while (countCharToSearch < cutIndex) {
                countCharToSearch++;
                index++;
            } // получили индекс index = cutIndex

            Right = Left;

            cutL = this->copy(cutIndex - index, index, max_);
            countCharToSearch -= index; // возвращаемся в начало блока первого символа

            // идём до блока с cut_index + search.len(); - следующий символ после искомого (возможно тот же блок)
            while (countCharToSearch + Right->now_ /*- 1*/ < cutIndex + search.len()) {
                countCharToSearch += Right->now_;
                Right = Right->next_;
            }

            // ищем индекс символа в блоке
            while (countCharToSearch < cutIndex + search.len()) {
                countCharToSearch++;
                end_index++;
            } // получили индекс end_index = cutIndex + search.len()

            cutR = this->copy(cutIndex + search.len(), Left->now_ - end_index, max_);


            // удаляем блоки. вставляем то что нужно нам
            // продолжаем поиск со следущего блока

            Right = Right->next_;     // Возможен NULL !!!
            helpL = Left->last_;        // Возможен NULL !!!
            // завязать first(Left)
            while (Left != Right) {
                helpForDel = Left->next_;
                del(Left);
                Left = helpForDel;
            }

            // left = right левая и правая часть склеины (SadCat)
            // нужно разорвать строку this на lefr и right
            Left = helpL; // Возможен NULL !!! // можно не делать, тк есть helpL который уже Left->last_
            // В данный момент Left и right могут быть nullptr
            // нужно склеить Left cutL copyreplace cutR Right
            // Left-х х-Right

//            if (Right != nullptr) { // завязка cutR и right
//                Right->last_ = cutR.dps_;
//                cutR.dps_->next_ = Right;
//            }

            cutL = cutL + copyReplace + cutR;

            if (Left != nullptr) { // завязка left и cutL
                Left->next_ = cutL.dps_;
                cutL.dps_->last_ = Left;
            }

            if (Left != nullptr) {
                while (Left->next_ != nullptr) {
                    Left = Left->next_;
                }
                Left->next_ = Right;
                if (Right != nullptr) Right->last_ = Left;
            }


            copyReplace.dps_ = nullptr;
            cutL.dps_ = nullptr;
            cutR.dps_ = nullptr;

            if (Right != nullptr) {
                this->dps_ = Right; // right может быть нуллптр
                cutIndex = this->search(search);
            } else {
                cutIndex = -1;
            }

        }

        while (this->dps_->last_ != nullptr){ //
            this->dps_ = this->dps_->last_;
        }

//        DPS *first;
//        DPS *second;
//        DPS *help_first;
//        DPS *help;
//        DPS *copy_help;
//        // ---
//        bool pupalupa = false;
//
//        // search
//        int cut_index = this->search(search);
//
//         while (cut_index != -1) {
//
//             MyString cut_1(max_);
//             MyString cut_2(max_);
//
//             MyString copy_replace(max_);
//             copy_replace = replase.copy(0,replase.len(),max_); // создание копии исходной строки replace
//             first = dps_;
//---------------------------------------------------------------------------------------------------------
//            int count_obj = 0; // кол-во символов до искомого
//
//            // идём до блока с cut_index;
//            while (count_obj + first->now_ - 1 < cut_index) {
//                count_obj += first->now_;
//                first = first->next_;
//            }
//          // count_obj смотрит на первый элемент нужного блока
//---------------------------------------------------------------------------------------------------------
//            int index = 0; // индекс начала в блоке
//            int end_index = 0;---
//
//            while (count_obj < cut_index) {
//                count_obj++;
//                index++;
//            } // получили индекс index = cut_index
//---------------------------------------------------------------------------------------------------------
//            second = first;---
//
//            cut_1 = this->copy(cut_index - index, index, max_);
//            count_obj -= index; // возвращаемся в начало блока первого символа
//
//            // идём до блока с cut_index + search.len(); - следующий символ после искомого (возможно тот же блок)
//            while (count_obj + second->now_ /*- 1*/ < cut_index + search.len()) {
//                count_obj += second->now_;
//                second = second->next_;
//            }
//
//            // ищем индекс символа в блоке
//            while (count_obj < cut_index + search.len()) {
//                count_obj++;
//                end_index++;
//            } // получили индекс end_index = cut_index + search.len()
//
//             cut_2 = this->copy(cut_index + search.len(), first->now_ - end_index, max_);
//---------------------------------------------------------------------------------------------------------
//            // удаляем блоки. вставляем то что нужно нам
//            // продолжаем поиск со следущего блока
//
//            second = second->next_;     // Возможен NULL !!!
//            help = first->last_;        // Возможен NULL !!!
//            // завязать first
//            while (first != second) {
//                help_first = first->next_;
//                del(first);
//                first = help_first;
//            }
// if (help == nullptr) { pupalupa = true; }
//// V------------------------ НАЧИНАЕТСЯ ТУТ ------------------------V
//
//            if (cut_1.dps_->now_ != 0) {                // связка левой части и первой вырезки
//                 if (help != nullptr) { connect(help, cut_1.dps_); }
//                 else { help = cut_1.dps_; }
//                help = cut_1.dps_;
//            }
//
//            if (copy_replace.dps_->now_ != 0 && copy_replace.dps_ != nullptr) {         // связка левой части и вставки
//
//                copy_help = copy_replace.dps_;
//                while (copy_help->next_ != nullptr) {
//                    copy_help = copy_help->next_;
//                }
//
//                if (help != nullptr) { connect(help, copy_replace.dps_); }
//                else { help = copy_replace.dps_; }
//
//                help = copy_help;
//
//            }
//
//             if (cut_2.dps_->now_ != 0) {         // связка левой части и второй вырезки
//
//                 if (help != nullptr) { connect(help, cut_2.dps_); }
//                 else { help = cut_2.dps_; }
//
//                 help = cut_2.dps_;
//
//             }
//
//
//             if (pupalupa) { // если оизменяем в первом блоке, то терям связь на second
//                 help->next_ = second;
//                 if (second != nullptr) {
//                     second->last_ = help;
//                     if (second != copy_replace.dps_) second = help;
//                 }
//             }
//
//
//            if (second != nullptr) this->dps_ = second; //????????
//            else this->dps_ = help;
//============================================================================================
//            copy_replace.dps_ = nullptr;
//            cut_1.dps_ = nullptr;
//            cut_2.dps_ = nullptr;
//
//// ^------------------------ ЗАКАНЬЧИВАЕТСЯ ТУТ ------------------------^
//            cut_index = this->search(search);
//// ^------------------------ ЗАКАНЬЧИВАЕТСЯ ТУТ ------------------------^
//
//             pupalupa = false;
//        }
//
//        while (this->dps_->last_ != nullptr){ //
//            this->dps_ = this->dps_->last_;
//        }=====================================================================================

    }

//    void SR (MyString &search /* то что ищем */, MyString &replase /* то на что меням  */) {
//        /*
//         *
//         * 1 - найти подстроку search
//         * 2 - удалить подстрок (возможные варианты: удаление внутри блока, удаление блоков, удаление части блока сначала и с конца)
//         *      делем исходную строку на две
//         * 3 - создание копии строки replace
//         * 4 - подстановка копии replace в исходную строку
//         * 5 - продолжение поиска со второй половины новой строки
//         */
//    }

};