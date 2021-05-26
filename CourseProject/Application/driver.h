#pragma once

class DriverClass {
public:
	DriverClass() = default;
	explicit DriverClass(const wchar_t* val) : val_{ ToEnum(val) }
	{
	}
	explicit operator const wchar_t* () const { return arr_[static_cast<std::size_t>(val_)]; }
	friend bool operator<(DriverClass lhs, DriverClass rhs) { return lhs.val_ < rhs.val_; }
private:
	enum class Val_ : std::size_t { _M, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13 };
	static constexpr std::array<const wchar_t*, 15U> arr_ = {
	L"M", L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"11", L"12", L"13"
	};

	Val_ ToEnum(std::wstring_view sv)
	{
		for (size_t i = 0U; i < arr_.size(); ++i)
			if (arr_[i] == sv) return static_cast<Val_>(i);
		throw std::invalid_argument("");
	}

	Val_ val_ = Val_::_3;
};

struct Driver
{
public:
	Driver() = default;

	std::wstring GetLastName() const { return LastName; }
	std::wstring GetFirstName() const { return FirstName; }
	std::wstring GetSecondName() const { return SecondName; }
	DriverClass GetDriverClass() const { return Class; }
	std::time_t GetEmploymentDate() const { return EmploymentDate; }
	std::size_t GetSalary() const { return Salary; }

	void SetLastName(const std::wstring& val) { LastName = val; }
	void SetFirstName(const std::wstring& val) { FirstName = val; }
	void SetSecondName(const std::wstring& val) { SecondName = val; }
	void SetDriverClass(DriverClass val) { Class = val; }
	void SetEmploymentDate(std::time_t val) { EmploymentDate = val; }
	void SetSalary(std::size_t val) { Salary = val; }

	std::wstring GetFullName() const { return LastName + L' ' + FirstName.front() + L'.' + SecondName.front() + L'.'; }
	std::wstring GetExperience() const {
		const std::time_t now = std::time(nullptr);
		auto diff = std::difftime(now, EmploymentDate) / 31'536'000;
		std::wostringstream ss;
		ss.imbue(std::locale(""));
		ss << std::setprecision(2) << diff;
		return ss.str();
	}

private:
	std::wstring LastName;       // Фамилия
	std::wstring FirstName;      // Имя
	std::wstring SecondName;     // Отчество
	DriverClass  Class;          // Класс водителя
	std::time_t  EmploymentDate; // Дата устройства
	std::size_t  Salary;         // оклад
};

