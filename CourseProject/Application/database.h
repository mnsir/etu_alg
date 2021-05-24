#pragma once

#include <list>
#include <vector>
#include <algorithm>

#include "driver.h"

class DataBase
{
public:
	DataBase() = default;

	bool Empty() const { return list_.empty(); }
	size_t Size() const { return list_.size(); }

	size_t Push(Driver&& driver) {
		auto it = list_.emplace(list_.cend(), std::move(driver));
		size_t pos = vector_.size();
		vector_.emplace_back(it);

		if (Update) Update();

		return pos;
	}

	Driver Pop(size_t pos) {
		auto it = vector_.begin();
		std::advance(it, pos);
		auto it_ = *it;
		vector_.erase(it);
		auto driver = std::move(*it_);
		list_.erase(it_);

		if (Update) Update();

		return driver;
	}

	const Driver& Get(size_t pos) const
	{
		return *vector_[pos];
	}

	std::function<void(void)> Update = nullptr;
	std::function<bool(const Driver&, const Driver&)> Predicate = nullptr;

	void Load()
	{
		using namespace std::literals::string_view_literals;
		pugi::xml_document xml;
		if (xml.load_file(file_))
		{
			auto doc = xml.document_element();
			for (auto node : doc.children())
			{
				auto it = list_.emplace(list_.cend());
				for (auto attribute : node.attributes())
				{
					if (attribute.name() == L"last_name"sv)
						it->SetLastName(attribute.value());
					else if (attribute.name() == L"first_name"sv)
						it->SetFirstName(attribute.value());
					else if (attribute.name() == L"second_name"sv)
						it->SetSecondName(attribute.value());
					else if (attribute.name() == L"driver_class"sv)
						it->SetDriverClass(DriverClass(attribute.value()));
					else if (attribute.name() == L"employment_date"sv)
					{
						it->SetEmploymentDate(attribute.as_llong(std::time(nullptr)));
					}
					else if (attribute.name() == L"salary"sv)
					{
						it->SetSalary(attribute.as_uint());
					}
				}
				vector_.emplace_back(std::move(it));
			}
			if (Update) Update();
		}
	}
	void Unload(bool save)
	{
		if (save)
		{
			pugi::xml_document xml;
			xml.append_child(L"document");

			auto doc = xml.document_element();
			for (const auto& driver : list_)
			{
				auto node = doc.append_child(L"driver");
				node.append_attribute(L"last_name").set_value(driver.GetLastName().c_str());
				node.append_attribute(L"first_name").set_value(driver.GetFirstName().c_str());
				node.append_attribute(L"second_name").set_value(driver.GetSecondName().c_str());
				node.append_attribute(L"driver_class").set_value(
					static_cast<const wchar_t*>(driver.GetDriverClass())
				);
				node.append_attribute(L"employment_date").set_value(driver.GetEmploymentDate());
				node.append_attribute(L"salary").set_value(driver.GetSalary());
			}
			xml.save_file(file_);
		}
		list_.clear();
		vector_.clear();
		if (Update) Update();
	}

	void Sort() {
		for (auto it = vector_.begin(); it != vector_.end(); it++)
		{
			auto const insertion_point = std::upper_bound(vector_.begin(), it, *it, 
				[this](decltype(vector_)::const_reference lhs, decltype(vector_)::const_reference rhs) {
					return Predicate(*lhs, *rhs);
				});
			std::rotate(insertion_point, it, it + 1);
		}
		Update();
	}

private:
	constexpr static const wchar_t* file_ = L"base.xml";
	std::list<Driver> list_;
	std::vector<decltype(list_)::iterator> vector_;
};

