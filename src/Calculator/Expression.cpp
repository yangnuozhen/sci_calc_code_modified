#include "Expression.h"

Expression::Expression()
{
    this->root = nullptr;
    this->isRPN = false;
    this->isValidSyntax = true;
}

Expression::Expression(Node *root, bool isRPN)
{
    this->root = root;
    this->isRPN = isRPN;
    this->isValidSyntax = true;
}

Expression::Expression(Node *root)
{
    this->root = root;
    this->isRPN = false;
    this->isValidSyntax = true;
}

Expression::Expression(std::string str, bool isRPN)
{
    this->isRPN = isRPN;
    this->isValidSyntax = true;
    if (this->isRPN)
    {
        this->root = buildTree(shuntingYard(parseStringRPN(str)));
    }
    else
    {
        this->root = buildTree(shuntingYard(parseString(str)));
    }
}

Expression::Expression(std::string str)
{
    this->root = buildTree(shuntingYard(parseString(str)));
    this->isRPN = false;
}

Expression::Expression(std::vector<Node> nodes, bool isRPN)
{
    this->isRPN = isRPN;
    this->isValidSyntax = true;
    if (this->isRPN)
    {
        this->root = buildTree(parseRPN(nodes));
    }
    else
    {
        this->root = buildTree(shuntingYard(nodes));
    }
}

Expression::Expression(std::vector<Node> nodes)
{
    this->isRPN = false;
    this->isValidSyntax = true;
    if (this->isRPN)
    {
        this->root = buildTree(parseRPN(nodes));
    }
    else
    {
        this->root = buildTree(shuntingYard(nodes));
    }
}

Node *const Expression::getRoot() const
{
    return this->root;
}

bool Expression::isValid() const
{
    return this->isValidSyntax && this->root != nullptr;
}

Expression Expression::operator+(const Expression &rhs) const
{
    addNode(Node(ADD, 0, this->root, rhs.getRoot()));
    int index = lastNode;
    return Expression(&nodeList[index]);
}

Expression Expression::operator-(const Expression &rhs) const
{
    addNode(Node(SUB, 0, this->root, rhs.getRoot()));
    int index = lastNode;
    return Expression(&nodeList[index]);
}

Expression Expression::operator*(const Expression &rhs) const
{
    addNode(Node(MUL, 0, this->root, rhs.getRoot()));
    int index = lastNode;
    return Expression(&nodeList[index]);
}

Expression Expression::operator/(const Expression &rhs) const
{
    addNode(Node(DIV, 0, this->root, rhs.getRoot()));
    int index = lastNode;
    return Expression(&nodeList[index]);
}

Expression Expression::operator^(const Expression &rhs) const
{
    addNode(Node(POW, 0, this->root, rhs.getRoot()));
    int index = lastNode;
    return Expression(&nodeList[index]);
}

std::vector<Node> Expression::parseString(std::string str)
{
    std::vector<Node> nodes;
    Serial.println("Start parsing");
    int prev = 0;
    this->isValidSyntax = true;
    for (int i = 0; i < str.size(); i++)
    {
        Serial.println(("Substring: " + str.substr(prev, i - prev + 1)).c_str());
        if (strIsOperator(str.substr(prev, i - prev + 1)))
        {
            Serial.println(str.substr(prev, i - prev + 1).c_str());
            if ((convertToEnum(str.substr(prev, i - prev + 1)) == SUB && nodes.size() == 0) || (convertToEnum(str.substr(prev, i - prev + 1)) == SUB && nodes[nodes.size() - 1].getToken() != NUM && nodes[nodes.size() - 1].getToken() != VAR && nodes[nodes.size() - 1].getToken() != RPARA))
            {
                nodes.push_back(Node(NEGATIVE, 0));
            }
            else
            {
                nodes.push_back(Node(convertToEnum(str.substr(prev, i - prev + 1)), 0));
            }
            prev = i + 1;
        }
        else if ('0' <= str[i] && str[i] <= '9' || str[i] == '.')
        {
            if (prev != i)
            {
                this->isValidSyntax = false;
                break;
            }
            Serial.println("number");
            long double res = 0;
            int j = i;
            bool pastDecimalPoint = false;
            long double cnt = 1;
            while (j < str.size() && (('0' <= str[j] && str[j] <= '9') || str[j] == '.'))
            {
                if (pastDecimalPoint)
                    cnt *= 10;
                if (str[j] == '.')
                    pastDecimalPoint = true;
                if ('0' <= str[j] && str[j] <= '9')
                    res = res * 10 + (str[j] - '0');
                j++;
            }
            res /= cnt;
            i = j - 1;
            nodes.push_back(Node(NUM, res));
            prev = i + 1;
        }
        else if (str[i] == 'x')
        {
            if (prev != i)
            {
                this->isValidSyntax = false;
                break;
            }
            nodes.push_back(Node(VAR, "x"));
            prev = i + 1;
        }
    }
    if (prev != str.size())
    {
        this->isValidSyntax = false;
    }
    // printNodeVector(nodes);
    // std::cout << std::endl;
    return nodes;
}

std::vector<Node> Expression::parseStringRPN(std::string str)
{
    std::vector<Node> nodes;
    int prev = 0;
    this->isValidSyntax = true;
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == ' ')
        {
            prev = i + 1;
            continue;
        }
        if (strIsOperator(str.substr(prev, i - prev + 1)))
        {
            if ((convertToEnum(str.substr(prev, i - prev + 1)) == SUB && nodes.size() == 0) || (convertToEnum(str.substr(prev, i - prev + 1)) == SUB && nodes[nodes.size() - 1].getToken() != NUM && nodes[nodes.size() - 1].getToken() != VAR && nodes[nodes.size() - 1].getToken() != RPARA))
            {
                nodes.push_back(Node(NEGATIVE, 0));
            }
            else
            {
                nodes.push_back(Node(convertToEnum(str.substr(prev, i - prev + 1)), 0));
            }
            prev = i + 1;
        }
        else if ('0' <= str[i] && str[i] <= '9')
        {
            if (prev != i)
            {
                this->isValidSyntax = false;
                break;
            }
            long double res = 0;
            int j = i;
            bool pastDecimalPoint = false;
            long double cnt = 1;
            while (j < str.size() && (('0' <= str[j] && str[j] <= '9') || str[j] == '.'))
            {
                if (pastDecimalPoint)
                    cnt *= 10;
                if (str[j] == '.')
                    pastDecimalPoint = true;
                if ('0' <= str[j] && str[j] <= '9')
                    res = res * 10 + (str[j] - '0');
                j++;
            }
            res /= cnt;
            i = j - 1;
            nodes.push_back(Node(NUM, res));
            prev = i + 1;
        }
        else if (str[i] == 'x')
        {
            if (prev != i)
            {
                this->isValidSyntax = false;
                break;
            }
            nodes.push_back(Node(VAR, "x"));
            prev = i + 1;
        }
        else
        {
            this->isValidSyntax = false;
            break;
        }
    }
    // printNodeVector(nodes);
    // std::cout << std::endl;
    if (prev != str.size())
    {
        this->isValidSyntax = false;
    }
    return nodes;
}

bool Expression::checkSyntax(std::vector<Node> nodes)
{
    if (!this->isValidSyntax)
    {
        return false;
    }
    if (nodes.empty())
    {
        Serial.println("empty expression");
        return false;
    }

    int paracnt = 0;
    bool expectsOperand = true;
    for (int i = 0; i < nodes.size(); i++)
    {
        Token token = nodes[i].getToken();

        if (token == NUM || token == VAR)
        {
            if (!expectsOperand)
            {
                Serial.println("unexpected operand");
                return false;
            }
            expectsOperand = false;
            continue;
        }
        if (token == LPARA)
        {
            if (!expectsOperand)
            {
                Serial.println("unexpected left para");
                return false;
            }
            paracnt++;
            continue;
        }
        if (token == RPARA)
        {
            if (expectsOperand)
            {
                Serial.println("unexpected right para");
                return false;
            }
            paracnt--;
            expectsOperand = false;
        }
        else if (token == NEGATIVE || nodes[i].isUnary())
        {
            if (!expectsOperand)
            {
                Serial.println("unexpected unary");
                return false;
            }
            continue;
        }
        else if (nodes[i].isBinary())
        {
            if (expectsOperand)
            {
                Serial.println("unexpected binary");
                return false;
            }
            expectsOperand = true;
        }
        else
        {
            Serial.println("unknown token");
            return false;
        }

        if (paracnt < 0)
        {
            Serial.println("para <0");
            return false;
        }
    }

    if (paracnt != 0)
    {
        Serial.println("para mismatch");
        return false;
    }
    if (expectsOperand)
    {
        Serial.println("expression ended early");
        return false;
    }
    Serial.println("Syntax true 2");
    return true;
}

bool Expression::checkSyntax(std::string str)
{
    std::vector<Node> nodes = parseString(str);
    if (!checkSyntax(nodes))
    {
        return false;
    }

    std::queue<Node> rpn = shuntingYard(nodes);
    Expression validationExpression;
    validationExpression.root = validationExpression.buildTree(rpn);
    return validationExpression.isValid();
}

std::queue<Node> Expression::shuntingYard(std::vector<Node> nodes)
{
    std::queue<Node> output;
    std::stack<Node> operators;
    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i].getToken() == NUM || nodes[i].getToken() == VAR)
        {
            output.push(nodes[i]);
        }
        if (nodes[i].isOperator())
        {
            if (nodes[i].isUnary())
            {
                operators.push(nodes[i]);
            }
            if (nodes[i].isBinary())
            {
                while (!operators.empty() && (operators.top().isUnary() || operators.top().getPrecedence() > nodes[i].getPrecedence() || (operators.top().getPrecedence() == nodes[i].getPrecedence() && nodes[i].isLeftAssociative())) && operators.top().getToken() != LPARA)
                {
                    output.push(operators.top());
                    operators.pop();
                }
                operators.push(nodes[i]);
            }
        }
        if (nodes[i].getToken() == LPARA)
        {
            operators.push(nodes[i]);
        }
        if (nodes[i].getToken() == RPARA)
        {
            while (!operators.empty() && operators.top().getToken() != LPARA)
            {
                output.push(operators.top());
                operators.pop();
            }
            operators.pop();
        }
    }
    while (!operators.empty())
    {
        output.push(operators.top());
        operators.pop();
    }
    return output;
}

std::queue<Node> Expression::parseRPN(std::vector<Node> nodes)
{
    std::queue<Node> output;
    for (auto i : nodes)
    {
        output.push(i);
    }
    return output;
}

Node *Expression::buildTree(std::queue<Node> rpn)
{
    if (rpn.empty())
    {
        this->isValidSyntax = false;
        return nullptr;
    }

    std::stack<Node *> stk;

    while (!rpn.empty())
    {
        Node cur = rpn.front();
        rpn.pop();
        addNode(cur);
        Node *currentNode = &nodeList[lastNode];

        if (cur.getToken() == NUM || cur.getToken() == VAR)
        {
            stk.push(currentNode);
        }
        else
        {
            if (cur.isUnary())
            {
                if (stk.empty())
                {
                    this->isValidSyntax = false;
                    return nullptr;
                }
                currentNode->setLeftChild(stk.top());
                stk.pop();
            }
            else if (cur.isBinary())
            {
                if (stk.size() < 2)
                {
                    this->isValidSyntax = false;
                    return nullptr;
                }
                currentNode->setRightChild(stk.top());
                stk.pop();
                currentNode->setLeftChild(stk.top());
                stk.pop();
            }
            else
            {
                this->isValidSyntax = false;
                return nullptr;
            }
            stk.push(currentNode);
        }
    }

    if (stk.size() != 1)
    {
        this->isValidSyntax = false;
        return nullptr;
    }

    return stk.top();
}

std::string Expression::getExpressionStringPointer(Node *node)
{
    if (node == nullptr)
    {
        return "";
    }

    std::string res = "";
    if (node->getToken() == NUM)
    {
        if (cmpLongDouble(node->getValue() - int(node->getValue()), 0))
            return std::to_string(int(node->getValue()));
        else
            return std::to_string(node->getValue());
    }
    if (node->getToken() == VAR)
    {
        return node->getName();
    }
    if (node->isOperator())
    {
        if (node->isUnary())
        {
            if (node->getLeftChild() == nullptr)
            {
                return tokenStr[node->getToken()] + "(?)";
            }
            res = tokenStr[node->getToken()] + "(" + getExpressionStringPointer(node->getLeftChild()) + ")";
        }
        else if (node->isBinary())
        {
            if (node->getLeftChild() == nullptr || node->getRightChild() == nullptr)
            {
                return "?";
            }

            if (node->getLeftChild()->getPrecedence() < node->getPrecedence())
            {
                res += "(" + getExpressionStringPointer(node->getLeftChild()) + ")";
            }
            else
            {
                res += getExpressionStringPointer(node->getLeftChild());
            }

            res += tokenStr[node->getToken()];

            if (node->getRightChild()->getPrecedence() < node->getPrecedence())
            {
                res += "(" + getExpressionStringPointer(node->getRightChild()) + ")";
            }
            else
            {
                res += getExpressionStringPointer(node->getRightChild());
            }
        }
    }
    return res;
}

std::string Expression::getExpressionString()
{
    return getExpressionStringPointer(this->root);
}

bool Expression::simplifyPointer(Node *node)
{ // WARNING!!! THIS FUNCTION WOULD MODIFY NODES THAT MIGHT BE LINKED TO PREVIOUS EXPRESSIONS!! IT IS ALWAYS RECOMMENDED TO DO COPYEXPRESSION FIRST, THEN SIMPLIFY ON THAT. true: has variable, false: no variables must do non variable first
    if (node == nullptr)
    {
        return false;
    }

    bool lson = false, rson = false;
    if (node->getLeftChild() != nullptr)
        lson = simplifyPointer(node->getLeftChild());
    if (node->getRightChild() != nullptr)
        rson = simplifyPointer(node->getRightChild());

    if (node->getToken() == NUM)
    {
        return false;
    }
    else if (node->getToken() == VAR)
    {
        return true;
    }
    else
    { // an operator

        if ((node->isUnary() && lson == false) || (node->isBinary() && lson == false && rson == false))
        { // all numbers
            node->printNode();
            long double res = 0;
            switch (node->getToken())
            {
            case NEGATIVE:
                res = -(node->getLeftChild()->getValue());
                break;
            case ADD:
                res = (node->getLeftChild()->getValue()) + (node->getRightChild()->getValue());
                break;
            case SUB:
                res = (node->getLeftChild()->getValue()) - (node->getRightChild()->getValue());
                break;
            case MUL:
                res = (node->getLeftChild()->getValue()) * (node->getRightChild()->getValue());
                break;
            case DIV:
                res = (node->getLeftChild()->getValue()) / (node->getRightChild()->getValue());
                break;
            case POW:
                res = pow(node->getLeftChild()->getValue(), (node->getRightChild()->getValue()));
                break;
            case SQRT:
                res = sqrtl(node->getLeftChild()->getValue());
                break;
            case LN:
                res = logl(node->getLeftChild()->getValue());
                break;
            case EXP:
                res = expl(node->getLeftChild()->getValue());
                break;
            case SIN:
                res = sinl(node->getLeftChild()->getValue());
                break;
            case COS:
                res = cosl(node->getLeftChild()->getValue());
                break;
            case TAN:
                res = tanl(node->getLeftChild()->getValue());
                break;
            case SEC:
                res = 1.0 / cosl(node->getLeftChild()->getValue());
                break;
            case CSC:
                res = 1.0 / sinl(node->getLeftChild()->getValue());
                break;
            case COT:
                res = 1.0 / tanl(node->getLeftChild()->getValue());
                break;
            case ASIN:
                res = asinl(node->getLeftChild()->getValue());
                break;
            case ACOS:
                res = acosl(node->getLeftChild()->getValue());
                break;
            case ATAN:
                res = atanl(node->getLeftChild()->getValue());
                break;
            default:
                res = 0;
            }
            *node = Node(NUM, res);
            return false;
        }
        else if (node->isUnary())
        { // unary operator
            // node -> setLeftChild(addNode(Node(NUM, res)));
            return true;
        }
        else
        { // binary operator
            // std::cout << "binary with x" << convertToString(node -> getToken()) << " " << lson.first << " " << lson.second << " " << rson.first << " " << rson.second << std::endl;
            if (node->getToken() == ADD)
            {
                if (lson == false && cmpLongDouble(node->getLeftChild()->getValue(), 0))
                {
                    // std::cout << "binary with x" << convertToString(node -> getToken()) << " " << lson.first << " " << lson.second << " " << rson.first << " " << rson.second << std::endl;
                    // std::cout << 1 << std::endl;
                    *node = *(node->getRightChild());
                    return true;
                }
                else if (rson == false && cmpLongDouble(node->getRightChild()->getValue(), 0))
                {
                    // std::cout << 2 << std::endl;
                    *node = *(node->getLeftChild());
                    return true;
                }
            }
            if (node->getToken() == SUB)
            {
                if (rson == false && cmpLongDouble(node->getRightChild()->getValue(), 0))
                {
                    // std::cout << 3 << std::endl;
                    *node = *(node->getLeftChild());
                    return true;
                }
            }
            if (node->getToken() == MUL)
            {
                if (lson == false && cmpLongDouble(node->getLeftChild()->getValue(), 0))
                {
                    // std::cout << "binary with x" << convertToString(node -> getToken()) << " " << lson.first << " " << lson.second << " " << rson.first << " " << rson.second << std::endl;

                    // std::cout << 4 << std::endl;
                    *node = *(node->getLeftChild());
                    return false;
                }
                else if (rson == false && cmpLongDouble(node->getRightChild()->getValue(), 0))
                {
                    // std::cout << 5 << std::endl;
                    *node = *(node->getRightChild());
                    return false;
                }
                else if (lson == false && cmpLongDouble(node->getLeftChild()->getValue(), 1))
                {
                    // std::cout << 6 << std::endl;
                    *node = *(node->getRightChild());
                    return true;
                }
                else if (rson == false && cmpLongDouble(node->getRightChild()->getValue(), 1))
                {
                    // std::cout << 7 << std::endl;
                    *node = *(node->getLeftChild());
                    return true;
                }
            }
            if (node->getToken() == DIV)
            {
                if (lson == false && cmpLongDouble(node->getLeftChild()->getValue(), 0))
                {
                    // std::cout << 4 << std::endl;
                    *node = *(node->getLeftChild());
                    return false;
                }
            }
            if (node->getToken() == POW)
            {
                if (rson == false && cmpLongDouble(node->getRightChild()->getValue(), 0))
                {
                    // std::cout << 5 << std::endl;
                    *node = *(node->getRightChild());
                    node->setValue(1);
                    return false;
                }
                else if (rson == false && cmpLongDouble(node->getRightChild()->getValue(), 1))
                {
                    // std::cout << 7 << std::endl;
                    *node = *(node->getLeftChild());
                    return true;
                }
            }
            return true;
        }
    }
}

void Expression::simplify()
{
    simplifyPointer(this->root);
}

long double Expression::evaluateExpressionPointer(Node *node)
{
    if (node == nullptr)
    {
        return 0;
    }

    switch (node->getToken())
    {
    case NUM:
        return node->getValue();
    case VAR:
    case NEGATIVE:
        return -evaluateExpressionPointer(node->getLeftChild());
    case ADD:
        return evaluateExpressionPointer(node->getLeftChild()) + evaluateExpressionPointer(node->getRightChild());
    case SUB:
        return evaluateExpressionPointer(node->getLeftChild()) - evaluateExpressionPointer(node->getRightChild());
    case MUL:
        return evaluateExpressionPointer(node->getLeftChild()) * evaluateExpressionPointer(node->getRightChild());
    case DIV:
        return evaluateExpressionPointer(node->getLeftChild()) / evaluateExpressionPointer(node->getRightChild());
    case POW:
        return pow(evaluateExpressionPointer(node->getLeftChild()), evaluateExpressionPointer(node->getRightChild()));
    case SQRT:
        return sqrtl(evaluateExpressionPointer(node->getLeftChild()));
    case LN:
        return logl(evaluateExpressionPointer(node->getLeftChild()));
    case EXP:
        return expl(evaluateExpressionPointer(node->getLeftChild()));
    case SIN:
        return sinl((angleMode ? 0.0174532925 : 1.0) * evaluateExpressionPointer(node->getLeftChild()));
    case COS:
        return cosl((angleMode ? 0.0174532925 : 1.0) * evaluateExpressionPointer(node->getLeftChild()));
    case TAN:
        return tanl((angleMode ? 0.0174532925 : 1.0) * evaluateExpressionPointer(node->getLeftChild()));
    case SEC:
        return 1.0 / cosl((angleMode ? 0.0174532925 : 1.0) * evaluateExpressionPointer(node->getLeftChild()));
    case CSC:
        return 1.0 / sinl((angleMode ? 0.0174532925 : 1.0) * evaluateExpressionPointer(node->getLeftChild()));
    case COT:
        return 1.0 / tanl((angleMode ? 0.0174532925 : 1.0) * evaluateExpressionPointer(node->getLeftChild()));
    case ASIN:
        return (angleMode ? 57.2957795 : 1.0) * asinl(evaluateExpressionPointer(node->getLeftChild()));
    case ACOS:
        return (angleMode ? 57.2957795 : 1.0) * acosl(evaluateExpressionPointer(node->getLeftChild()));
    case ATAN:
        return (angleMode ? 57.2957795 : 1.0) * atanl(evaluateExpressionPointer(node->getLeftChild()));
    default:
        return 0;
    }
}

long double Expression::evaluateExpression()
{
    if (!isValid())
    {
        return 0;
    }
    return evaluateExpressionPointer(this->root);
}

long double Expression::evaluateWithVariable(std::string variable, long double value)
{
    return evaluateWithVariablePointer(this->root, variable, value);
}

long double Expression::evaluateWithVariablePointer(Node *node, std::string variable, long double value)
{
    if (node == nullptr)
    {
        return 0;
    }

    if (node->getToken() == NUM)
    {
        return node->getValue();
    }
    else if (node->getToken() == VAR)
    {
        if (node->getName() == variable)
        {
            return value;
        }
        else
        {
            // 如果变量名不匹配，返回0或抛出错误
            return 0;
        }
    }
    else if (node->isOperator())
    {
        long double leftValue = evaluateWithVariablePointer(node->getLeftChild(), variable, value);
        long double rightValue = evaluateWithVariablePointer(node->getRightChild(), variable, value);

    switch (node->getToken()) {
        case NUM:
            return node->getValue();
        case VAR:
            if (node->getName() == variable) {
                return value;
            } else {
                // 如果变量名不匹配，返回0或抛出错误
                return 0;
            }
        case ADD:
            return evaluateWithVariablePointer(node->getLeftChild(), variable, value) +
                   evaluateWithVariablePointer(node->getRightChild(), variable, value);
        case SUB:
            return evaluateWithVariablePointer(node->getLeftChild(), variable, value) -
                   evaluateWithVariablePointer(node->getRightChild(), variable, value);
        case MUL:
            return evaluateWithVariablePointer(node->getLeftChild(), variable, value) *
                   evaluateWithVariablePointer(node->getRightChild(), variable, value);
        case DIV:
            return evaluateWithVariablePointer(node->getLeftChild(), variable, value) /
                   evaluateWithVariablePointer(node->getRightChild(), variable, value);
        case POW:
            return pow(evaluateWithVariablePointer(node->getLeftChild(), variable, value),
                       evaluateWithVariablePointer(node->getRightChild(), variable, value));
        case SQRT:
            return sqrt(evaluateWithVariablePointer(node->getLeftChild(), variable, value));
        case LN:
            return log(evaluateWithVariablePointer(node->getLeftChild(), variable, value));
        case EXP:
            return exp(evaluateWithVariablePointer(node->getLeftChild(), variable, value));
        case SIN:
            return sin(evaluateWithVariablePointer(node->getLeftChild(), variable, value));
        case COS:
            return cos(evaluateWithVariablePointer(node->getLeftChild(), variable, value));
        case TAN:
            return tan(evaluateWithVariablePointer(node->getLeftChild(), variable, value));
        case SEC:
            return 1 / cos(evaluateWithVariablePointer(node->getLeftChild(), variable, value));
        case CSC:
            return 1 / sin(evaluateWithVariablePointer(node->getLeftChild(), variable, value));
        case COT:
            return 1 / tan(evaluateWithVariablePointer(node->getLeftChild(), variable, value));
        case ASIN:
            return asin(evaluateWithVariablePointer(node->getLeftChild(), variable, value));
        case ACOS:
            return acos(evaluateWithVariablePointer(node->getLeftChild(), variable, value));
        case ATAN:
            return atan(evaluateWithVariablePointer(node->getLeftChild(), variable, value));
        default:
            // 未知操作符，返回0或抛出错误
            return 0;
    }
    }
    else if (node->isUnary())
    {
        long double childValue = evaluateWithVariablePointer(node->getLeftChild(), variable, value);

        switch (node->getToken())
        {
        case NEGATIVE:
            return -childValue;
        default:
            // 未知一元操作符，返回0或抛出错误
            return 0;
        }
    }
    else
    {
        // 未知节点类型，返回0或抛出错误
        return 0;
    }
}

Expression Expression::derivativePointer(Node *node)
{ // fix this later
    switch (node->getToken())
    {
    case NUM:
        return Expression(addNode(Node(NUM, 0)));
    case VAR:
        return Expression(addNode(Node(NUM, 1)));
    case ADD:
        return derivativePointer(node->getLeftChild()) + derivativePointer(node->getRightChild());
    case SUB:
        return derivativePointer(node->getLeftChild()) - derivativePointer(node->getRightChild());
    case MUL:
        return (derivativePointer(node->getLeftChild()) * Expression(node->getRightChild()) + derivativePointer(node->getRightChild()) * Expression(node->getLeftChild()));
    case DIV:
        return ((derivativePointer(node->getLeftChild()) * Expression(node->getRightChild()) - derivativePointer(node->getRightChild()) * Expression(node->getLeftChild())) / (Expression(node->getRightChild()) ^ Expression("2")));
    case POW:
        return (Expression(node->getRightChild()) * (Expression(node->getLeftChild()) ^ (Expression(node->getRightChild()) - Expression("1"))) * derivativePointer(node->getLeftChild()));
    case SQRT:
        return (Expression("1") / (Expression("2") * Expression(addNode(Node(SQRT, 0, node->getLeftChild(), nullptr))) * derivativePointer(node->getLeftChild())));
    case LN:
        return (Expression("1") / Expression(node->getLeftChild()) * derivativePointer(node->getLeftChild()));
    case EXP:
        return Expression(addNode(Node(EXP, 0, node->getLeftChild(), nullptr))) * derivativePointer(node->getLeftChild());
    case SIN:
        return Expression(addNode(Node(COS, 0, node->getLeftChild(), nullptr))) * derivativePointer(node->getLeftChild());
    case COS:
        return Expression("0") - Expression(addNode(Node(SIN, 0, node->getLeftChild(), nullptr))) * derivativePointer(node->getLeftChild());
    case TAN:
        return Expression(addNode(Node(SEC, 0, node->getLeftChild(), nullptr))) ^ Expression("2") * derivativePointer(node->getLeftChild());
    case SEC:
        return Expression(addNode(Node(SEC, 0, node->getLeftChild(), nullptr))) * Expression(addNode(Node(TAN, 0, node->getLeftChild(), nullptr))) * derivativePointer(node->getLeftChild());
    case CSC:
        return (Expression("0") - Expression(addNode(Node(CSC, 0, node->getLeftChild(), nullptr)))) * Expression(addNode(Node(COT, 0, node->getLeftChild(), nullptr))) * derivativePointer(node->getLeftChild());
    case COT:
        return Expression("0") - (Expression(addNode(Node(CSC, 0, node->getLeftChild(), nullptr))) ^ Expression("2")) * derivativePointer(node->getLeftChild());
    // case ASIN: return Expression("1") / Expression(addNode(Node(SQRT, 0, (), nullptr)));
    // case ACOS: return acosl(evaluateExpressionPointer(node -> getLeftChild()));
    // case ATAN: return atanl(evaluateExpressionPointer(node -> getLeftChild()));
    default:
        return 0;
    }
}

Expression Expression::derivative()
{
    return derivativePointer(this->root);
}

Expression syntaxChecker;
