#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;

struct Node
{
    string value;
    Node *left;
    Node *right;
};
typedef struct Node *Tree;

void init_Tree(Tree &t)
{
    t = NULL;
}

Node *new_node(string data)
{
    Node *p = new Node;
    p->value = data;
    p->left = nullptr;
    p->right = nullptr;
    return p;
}

bool laToanTu(string c)
{
    if (c == "+" || c == "-" || c == "*" || c == "/")
    {
        return true;
    }
    return false;
}

int do_uu_tien(string s)
{
    switch (s[0])
    {
    case '*':
    case '/':
        return 2;
    case '+':
    case '-':
        return 1;
    default:
        return 0;
    }
}

// Tạo cây biểu thức từ mảng hậu tố
void tao_cay_bieu_thuc(Tree &root, vector<string> mangHauTo)
{
    stack<Tree> s;
    for (string &item : mangHauTo)
    {
        if (laToanTu(item))
        {
            Node *pToanTu = new_node(item);
            pToanTu->right = s.top();
            s.pop();
            pToanTu->left = s.top();
            s.pop();
            s.push(pToanTu);
        }
        else
        {
            Node *pToanHang = new_node(item);
            s.push(pToanHang);
        }
    }
    root = s.top();
}

// Tính giá trị từ cây biểu thức
float gia_tri_cay_bieu_thuc(Tree root)
{
    if (root == nullptr)
    {
        return 0;
    }
    if (root->left == nullptr && root->right == nullptr) // kiểm tra nếu là nút lá thì chuyển sang kiểu số để tính toán
    {
        return stof(root->value); // chuyển chuỗi số sang kiểu số thực -- vd: "3"  --->   3.0
    }

    float giaTriTrai = gia_tri_cay_bieu_thuc(root->left);
    float giaTriPhai = gia_tri_cay_bieu_thuc(root->right);

    if (root->value == "+")
    {
        return giaTriTrai + giaTriPhai;
    }
    else if (root->value == "-")
    {
        return giaTriTrai - giaTriPhai;
    }
    else if (root->value == "*")
    {
        return giaTriTrai * giaTriPhai;
    }
    else
    {
        return giaTriTrai / giaTriPhai;
    }
}

//=========================================Ký pháp nghịch đảo ba lan=====================================================//
// Chuyển đổi sang hậu tố
vector<string> chuyen_sang_hau_to(vector<string> mangKyTu) // 215	6 5 15 5 / * 4 - * + 2 3 * +
{
    stack<string> s;
    vector<string> mangHauTo;

    for (int i = 0; i < mangKyTu.size(); i++)
    {
        string kiTuXet = mangKyTu[i];
        if (!(laToanTu(kiTuXet) || kiTuXet == "(" || kiTuXet == ")")) // TH là toán hạng
        {
            mangHauTo.push_back(kiTuXet);
        }
        else if (laToanTu(kiTuXet)) // TH là toán tử
        {
            while (!s.empty() && s.top() != "(" && do_uu_tien(s.top()) >= do_uu_tien(kiTuXet))
            {
                mangHauTo.push_back(s.top());
                s.pop();
            }
            s.push(kiTuXet);
        }
        else if (kiTuXet == "(") // trường hợp là "("
        {
            s.push(kiTuXet);
        }
        else if (kiTuXet == ")") // trường hợp ")"
        {
            while (!s.empty() && s.top() != "(")
            {
                mangHauTo.push_back(s.top());
                s.pop();
            }
            if (!s.empty() && s.top() == "(")
            {
                s.pop();
            }
        }
    }

    while (!s.empty())
    {
        mangHauTo.push_back(s.top());
        s.pop();
    }

    return mangHauTo;
}

// Từ chuỗi nhập vào phân tách thành một mảng string gồm các phần tử toán hạng, toán tử, dấu "(", dấu “)”.
vector<string> chuyen_sang_mang_ky_tu(string bieuThuc)
{
    vector<string> mangKyTu;
    int length = bieuThuc.length();
    bool laDauTruDauTien;

    for (int i = 0; i < length; i++)
    {
        string c1 = string(1, bieuThuc[i]);
        string c0 = string(1, bieuThuc[i - 1]);

        // kiểm tra phần tử đang xét có phải là dấu trừ đầu tiên không
        if (bieuThuc[i] == '-' && i == 0)
        {
            laDauTruDauTien = true;
        }
        else
        {
            laDauTruDauTien = false;
        }

        // Nếu là "(",")" ----> thêm
        // Nếu là (toán tử) đồng thời (phần tử trước đó không là toán tử, không là dấu '('  ) và (phan tu dang xet không là dấu trừ đầu tiên) ---> thêm
        if ((laToanTu(c1) && !laToanTu(c0) && c0 != "(" && !laDauTruDauTien) || c1 == "(" || c1 == ")")
        {
            mangKyTu.push_back(c1);
        }
        else
        {
            string temp = "";
            temp += c1;
            int count = 0;
            for (int j = i + 1; j < length; j++)
            {
                string c2 = string(1, bieuThuc[j]);
                if (!(laToanTu(c2) || c2 == "(" || c2 == ")")) // toan hang
                {
                    temp += c2;
                    count++;
                }
                else if (laToanTu(c1) && !(laToanTu(c2) || c2 == "(" || c2 == ")"))
                {
                    temp += c2;
                    count++;
                }
                else
                {
                    break;
                }
            }
            mangKyTu.push_back(temp);
            i += count;
        }
    }
    return mangKyTu;
}

void printArray(vector<string> arr)
{
    cout << "{";
    for (string &item : arr)
    {
        cout << " " << item << " ";
    }
    cout << "}";
}

int main()
{
    string bieuThuc;
    do
    {
        cout << "\n--------------------------------------------------------------------------------\n";
        cout << "Vui long nhap dung bieu thuc va khong khoang trong, nhap 'thoat' de dung tinh:\n";
        cout << "Nhap bieu thuc: ";
        cin >> bieuThuc;
        if (bieuThuc == "thoat")
            break;

        vector<string> mangKyTu = chuyen_sang_mang_ky_tu(bieuThuc);
        vector<string> mangHauTo = chuyen_sang_hau_to(mangKyTu);

        cout << "Mang ky tu: ";
        printArray(mangKyTu);

        cout << "\nMang hau to: ";
        printArray(mangHauTo);

        Tree root;
        init_Tree(root);
        tao_cay_bieu_thuc(root, mangHauTo);

        float ketQua = gia_tri_cay_bieu_thuc(root);
        cout << "\n>> Ket qua: " << bieuThuc << " = " << ketQua << endl;
    } while (1);

    return 0;
}