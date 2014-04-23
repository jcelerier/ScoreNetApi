#pragma once

#include "Group.h"
#include "../Iterable.h"
#include <utility>
#include <algorithm>
class GroupManager : public Iterable<Group>
{
		using SignalHandler = std::function<void()>;

	private:
		int _lastId = 0;
		Group::GroupSignalHandler groupChanged; //Même handler pour tous les groupes
		SignalHandler changed;

	public:
		GroupManager(Group::GroupSignalHandler onGroupChange, SignalHandler onChange):
			groupChanged(onGroupChange),
			changed(onChange)
		{
		}

		Group& createGroup(std::string name)
		{
			performUniquenessCheck(std::forward<std::string>(name));
			auto& g = create(name, _lastId++, groupChanged);

			changed();
			return g;
		}

		Group& createGroup(int id, std::string name)
		{
			performUniquenessCheck(id);
			performUniquenessCheck(std::forward<std::string>(name));
			auto& g = create(name, id, groupChanged);

			_lastId = std::max(_lastId, id);

			changed();
			return g;
		}

		template<typename... K>
		void remove(K&&... args)
		{
			 Iterable<Group>::remove(std::forward<K>(args)...);
			 changed();
		}
};
