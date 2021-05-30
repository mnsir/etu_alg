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

	std::wstring GetLastName() const { return lastName_; }
	std::wstring GetFirstName() const { return firstName_; }
	std::wstring GetSecondName() const { return secondName_; }
	DriverClass GetDriverClass() const { return class_; }
	std::time_t GetEmploymentDate() const { return employmentDate_; }
	std::size_t GetSalary() const { return salary_; }

	void SetLastName(const std::wstring& val) { lastName_ = val; }
	void SetFirstName(const std::wstring& val) { firstName_ = val; }
	void SetSecondName(const std::wstring& val) { secondName_ = val; }
	void SetDriverClass(DriverClass val) { class_ = val; }
	void SetEmploymentDate(std::time_t val) { employmentDate_ = val; }
	void SetSalary(std::size_t val) { salary_ = val; }

	std::wstring GetFullName() const { return lastName_ + L' ' + firstName_.front() + L'.' + secondName_.front() + L'.'; }
	std::wstring GetExperience() const {
		const std::time_t now = std::time(nullptr);
		auto diff = std::difftime(now, employmentDate_) / 31'536'000;
		std::wostringstream ss;
		ss.imbue(std::locale(""));
		ss << std::setprecision(2) << diff;
		return ss.str();
	}

private:
	std::wstring lastName_;       // Фамилия
	std::wstring firstName_;      // Имя
	std::wstring secondName_;     // Отчество
	DriverClass  class_;          // Класс водителя
	std::time_t  employmentDate_; // Дата устройства
	std::size_t  salary_;         // оклад
};

