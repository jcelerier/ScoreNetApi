#pragma once
#include <memory>
#include <vector>
#include"../properties/hasId.h"
#include"../properties/hasName.h"

class DistributedScenario;
class Group: public hasName, public hasId
{
	public:
		using hasName::hasSame;
		using hasId::hasSame;

		Group(std::string name, int id):
			hasName(name),
			hasId(id)
		{
		}

		~Group() = default;

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
		// TODO Changer en activation de la permission d'exécution avec client local
		void mute() { _isMuted = true; }
		void unmute() { _isMuted = false; }
		bool isMuted() { return _isMuted; }

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
			}
		}

	private:
		std::vector<std::reference_wrapper<const DistributedScenario> > _scenarios;
		bool _isMuted;
		// Couleur ?
};

using Group_p = std::shared_ptr<Group>;

// Policies : nombre max / 1 / indéterminé
// List de groupes ?
// Permissions : visibilité ? exécution ?

// Si groupe n'a pas de client : exécuter sur maître ? / ne pas exécuter ?
