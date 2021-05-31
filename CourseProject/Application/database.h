#pragma once

#include <list>
#include <vector>
#include <algorithm>

#include "driver.h"


// Элемент по DDD по логике не может иметь свойство типа Идентификатора, это
// понятие существует относительно контейнера, в котором хранится этот Элемент.
// Тогда Item - это Обертка Элемента, которая имеет Идентификатор и
// наследует от Типа Элемента все его геттеры
template <typename T>
class Item : public T
{
public:
	Item()
		: T{}
		, id { next_id_++ }
	{
	}

	Item(const Item<T>&) = delete;
	Item& operator=(const Item<T>&) = delete;
	Item(Item<T>&&) = default;
	Item& operator=(Item<T>&&) = default;
	// Получить Идендификатор
	size_t GetId() const { return id; }
	// Сбросить счетчика
	static void ResetIdCounter() { next_id_ = 0; }

private:
	inline static size_t next_id_ = 0; // Значение следующего Идендификатора
	size_t id; // Идендификатор
};

// База
template <typename T>
class DataBase// : public IDataBase
{
public:
	using Item = Item<T>;

	DataBase() = default;
	// Создает новый пустой Элемент в Базе и возвращает ссылку на него
	Item& CreateItem() { return items.emplace_back(Item()); }
	// Удаляет Элемент из Базы по Идентификатору
	void RemoveItem(size_t id)
	{
		auto it = std::remove_if(items.begin(), items.end(),
			[&id](const Item& item) { return item.GetId() == id; });
		items.erase(it);
	}
	// Удалить все Элементы из Базы
	void Clear() { items.clear(); }
	// Сортировка вставками
	// Вызов -> db.Sort(&decltype(db)::Item:: *** Member Function *** );
	// for - O(n)
	// rotate - O(n) upper_bound - O(log(n))
	// вместе O(n^2)
	template<typename RetTy>
	void Sort(RetTy(Item::* qwe)() const)
	{
		auto mf = std::mem_fn(qwe);
		auto pred = [&](const Item& lhs, const Item& rhs) {
			return mf(lhs) < mf(rhs);
		};
		for (auto it = items.begin(); it != items.end(); it++)
			std::rotate(
				std::upper_bound(items.begin(), it, *it, pred), 
				it, 
				it + 1);
	}
	// Получить список входящих Элементов в порядке последней примененной сортировки
	const std::vector<Item>& GetItems() const { return items; }
	// Бинарный поиск
	// Sort - O(n^2)
	// lower_bound - O(log(n))
	Item* Search(size_t id)
	{
		Sort(&Item::GetId);
		auto it = std::lower_bound(items.begin(), items.end(), id,
			[](const Item& lhs, size_t rhs) { return lhs.GetId() < rhs; });
		return (it != items.end()) ? &(*it) : nullptr;
	}

private:
	std::vector<Item> items; // Контейнер Элементов
};


template <typename T>
void Load(DataBase<T>& db, const wchar_t* fileName)
{
	using namespace std::literals::string_view_literals;

	pugi::xml_document xml;
	if (xml.load_file(fileName))
	{
		auto doc = xml.document_element();
		for (auto node : doc.children())
		{
			auto& driver = db.CreateItem();
			for (auto attribute : node.attributes())
			{
				if (attribute.name() == L"last_name"sv)
					driver.SetLastName(attribute.value());
				else if (attribute.name() == L"first_name"sv)
					driver.SetFirstName(attribute.value());
				else if (attribute.name() == L"second_name"sv)
					driver.SetSecondName(attribute.value());
				else if (attribute.name() == L"driver_class"sv)
					driver.SetDriverClass(DriverClass(attribute.value()));
				else if (attribute.name() == L"employment_date"sv)
				{
					driver.SetEmploymentDate(attribute.as_llong(std::time(nullptr)));
				}
				else if (attribute.name() == L"salary"sv)
				{
					driver.SetSalary(attribute.as_uint());
				}
			}
		}
	}
}

template <typename T>
void Unload(DataBase<T>& db, std::wstring fileName = L"")
{
	if (!fileName.empty())
	{
		pugi::xml_document xml;
		xml.append_child(L"document");

		auto doc = xml.document_element();
		db.Sort(&DataBase<T>::Item::GetId);
		for (const auto& driver : db.GetItems())
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
		xml.save_file(fileName.c_str());
	}
	db.Clear();
}
