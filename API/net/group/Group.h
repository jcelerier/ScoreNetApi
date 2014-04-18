#pragma once
#include <memory>
#include <vector>
#include"../properties/hasId.h"
#include"../properties/hasName.h"
class ClientCountPolicy
{
	// déterminé / indéterminé. Par défaut, 1 max.
};

class DistributedScenario;
class Group: public hasName, public hasId
{
	public:
		using hasName::hasSame;
		using hasId::hasSame;
		using GroupSignalHandler = std::function<void(Group&)>;
		using SignalHandler = std::function<void()>;

		Group(std::string name, int id, GroupSignalHandler changeHandler):
			hasName(name),
			hasId(id),
			changed(std::bind(changeHandler, std::ref(*this)))
		{
		}

		~Group()
		{
			if(changed)
				changed();
		}

		Group(Group&&) = default;
		Group(const Group&) = delete;
		Group& operator=(Group&& g) = default;
		Group& operator=(const Group&) = delete;

		bool operator==(const Group& g) const
		{
			return g.getId() == getId();
		}

		bool operator!=(const Group& g) const
		{
			return g.getId() != getId();
		}

		// Example de fonctionnalité que peut offrir les Groupes ?
		void mute() { }
		void unmute() { }

		std::vector<std::reference_wrapper<const DistributedScenario> >& getScenarios()
		{
			return _scenarios;
		}

		void removeScenario(const DistributedScenario& d)
		{
			_scenarios.erase
					(std::remove_if(begin(_scenarios),
						   end(_scenarios),
						   [&d] (std::reference_wrapper<const DistributedScenario> refSc)
			{
				return &refSc.get() == &d;
			}), end(_scenarios));

			changed();
		}

		void addScenario(const DistributedScenario& d)
		{
			if(std::none_of(begin(_scenarios),
							end(_scenarios),
							[&d] (std::reference_wrapper<const DistributedScenario> refSc)
						   {
							   return &refSc.get() == &d;
						   }))
			{
				_scenarios.emplace_back(d);
				changed();
			}
		}

		SignalHandler changed;

	private:
		ClientCountPolicy cp;
		std::vector<std::reference_wrapper<const DistributedScenario> > _scenarios;
		// Couleur ?
};

using Group_p = std::shared_ptr<Group>;

// Policies : nombre max / 1 / indéterminé
// List de groupes ?
// Permissions : visibilité ? exécution ?

// Si groupe n'a pas de client : exécuter sur maître ? / ne pas exécuter ?
