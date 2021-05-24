#pragma once

class DriverClass {
public:
	DriverClass() = default;
	explicit DriverClass(const wchar_t* val) : val_{ ToEnum(val) }
	{
	}
	explicit operator const wchar_t* () const { return arr_[static_cast<std::size_t>(val_)]; }
	bool operator<(DriverClass other) { return val_ < other.val_; }
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

class Driver
{
public:
	Driver() = default;
	Driver(const Driver&) = delete;
	Driver& operator=(const Driver&) = delete;
	Driver(Driver&&) = default;
	Driver& operator=(Driver&&) = default;

	std::wstring GetLastName() const { return last_name_; }
	std::wstring GetFirstName() const { return first_name_; }
	std::wstring GetSecondName() const { return second_name_; }
	DriverClass GetDriverClass() const { return driver_class_; }
	std::time_t GetEmploymentDate() const { return employment_date_; }
	std::size_t GetSalary() const { return salary_; }

	void SetLastName(const std::wstring& val) { last_name_ = val; }
	void SetFirstName(const std::wstring& val) { first_name_ = val; }
	void SetSecondName(const std::wstring& val) { second_name_ = val; }
	void SetDriverClass(DriverClass val) { driver_class_ = val; }
	void SetEmploymentDate(std::time_t val) { employment_date_ = val; }
	void SetSalary(std::size_t val) { salary_ = val; }

	std::wstring GetFullName() const { return last_name_ + L' ' + first_name_.front() + L'.' + second_name_.front() + L'.'; }
	std::wstring GetExperience() const {
		const std::time_t now = std::time(nullptr);
		auto diff = std::difftime(now, employment_date_) / 31'536'000;
		std::wostringstream ss;
		ss.imbue(std::locale(""));
		ss << std::setprecision(2) << diff;
		return ss.str();
	}

private:
	std::wstring last_name_; // Фамилия
	std::wstring first_name_; // Имя
	std::wstring second_name_; // Отчество
	DriverClass driver_class_; // Класс водителя
	std::time_t employment_date_; // Дата устройства
	std::size_t salary_;
};

