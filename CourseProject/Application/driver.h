#pragma once
class Driver
{
public:
	Driver() = default;

	std::wstring GetLastName() const { return last_name_; }
	std::wstring GetFirstName() const { return first_name_; }
	std::wstring GetSecondName() const { return second_name_; }
	std::wstring GetDriverClass() const { return driver_class_; }
	std::wstring GetEmploymentDate() const { return employment_date_; }
	std::wstring GetSalary() const { return salary_; }

	void SetLastName(const std::wstring& val) { last_name_ = val; }
	void SetFirstName(const std::wstring& val) { first_name_ = val; }
	void SetSecondName(const std::wstring& val) { second_name_ = val; }
	void SetDriverClass(const std::wstring& val) { driver_class_ = val; }
	void SetEmploymentDate(const std::wstring& val) { employment_date_ = val; }
	void SetSalary(const std::wstring& val) { salary_ = val; }

private:
	std::wstring last_name_;
	std::wstring first_name_;
	std::wstring second_name_;
	std::wstring driver_class_;
	std::wstring employment_date_;
	std::wstring salary_;
};

